#include "Include/Colors.hlsl"
#
#define LAST_UNKNOWN_AXIS   -1
#define LAST_X_AXIS         0
#define LAST_Y_AXIS         1
#define LAST_Z_AXIS         2

#define HIT_TYPE_UNKNOWN    0
#define HIT_TYPE_VOXEL      -1
#define HIT_TYPE_OBJECT     1

#define MAX_RENDER_DIST         28.f
#define HALF_MAX_RENDER_DIST    10.f
#define MAX_STEPS               73
#define BASE_SKY_COLOR          float4(.4078, .4725, 1., 1.)

struct PushConstants
{
    float3      CameraPos;
    uint        _Padding0;
    int3        GridSize;
    uint        _Padding1;
    float3      CameraLookDir;
    uint        _Padding2;
    float3      CameraRight;
    uint        _Padding3;
    float3      CameraUp;
    uint        _Padding4;
    float       fFov;
    uint        uDebugMode;
    uint        _Padding6;
    uint        _Padding7;
};

struct Voxel 
{
    uint    Type;
    uint    Color;
    uint    Id[26];
};

RWTexture2D< float4 >       g_OutputImage   : register( u0 );
StructuredBuffer< Voxel >   g_vVoxels       : register( t1 );
StructuredBuffer< float4 >  g_vPositions    : register( t2 );
StructuredBuffer< float4 >  g_vRotations    : register( t3 );
StructuredBuffer< float4 >  g_vHalfSizes    : register( t4 );

#if defined( VULKAN )

    [[ vk::push_constant ]]
    PushConstants pc;

#else

    cbuffer PushConstantsBuffer : register( b1 )
    {
        PushConstants pc;
    };

#endif

// --------------------------------------------------------------------------------------------------------------------
bool MarchTheRay( in const float3    ro,
                  in const float3    rd,
                  in const int       maxSteps,
                  out float3         hitCoords,
                  out uint           hitIndex,
                  out float          fDistance,
                  out float3         normal,
                  out int            hitType )
{
    int3 voxel = int3( ro );
    int3 step  = int3( sign( rd ));

    float3 tDelta = abs( 1.0 / rd );
    float3 tMax;
    
    // Calc initial offset
    float offset;
    for ( int i = 0; i < 3; ++i ) 
    {
        offset = rd[ i ] > 0.0 ? 1.0 - frac( ro[ i ] ) : frac( ro[ i ] );
        tMax[ i ] = tDelta[ i ] * offset;
    }

    int lastStepAxis = LAST_UNKNOWN_AXIS;
    int index;
    uint testedVoxel;
    bool stepX;
    bool stepY;
    for ( int stepCount = 0; stepCount < maxSteps; ++stepCount )
    {
        if ( voxel.x <= 0 || voxel.x >= pc.GridSize.x ||
             voxel.y <= 0 || voxel.y >= pc.GridSize.y ||
             voxel.z <= 0 || voxel.z >= pc.GridSize.z )
        {
            return false;
        }

        index = voxel.x + 
                voxel.y * pc.GridSize.x +
                voxel.z * pc.GridSize.x * pc.GridSize.y;

        // Check for hits
        testedVoxel = g_vVoxels[ index ].Type;
    
        if ( testedVoxel == uint( HIT_TYPE_VOXEL ) )
        {
            fDistance               = tMax[ lastStepAxis ] - tDelta[ lastStepAxis ];
            normal                  = float3( 0., 0., 0. );
            normal[ lastStepAxis ]  = -float( step[ lastStepAxis ] );

            hitIndex    = index;
            hitCoords   = ro + rd * fDistance;
            hitType     = HIT_TYPE_VOXEL;

            return true;
        } 
        if ( pc.uDebugMode == 1                 &&
             testedVoxel > HIT_TYPE_UNKNOWN     && 
             testedVoxel < uint( HIT_TYPE_VOXEL ) )
        {
            fDistance               = tMax[ lastStepAxis ] - tDelta[ lastStepAxis ];
            normal                  = float3( 0., 0., 0. );
            normal[ lastStepAxis ]  = -float( step[ lastStepAxis ] );

            hitIndex    = index;
            hitCoords   = ro + rd * fDistance;
            hitType     = HIT_TYPE_VOXEL;

            return true;
        }

        // Move the ray
        stepX = ( tMax.x < tMax.y ) && ( tMax.x < tMax.z );
        stepY = ( tMax.y < tMax.z );

        if ( stepX )
        {
            voxel.x  += step.x;
            tMax.x   += tDelta.x;
            lastStepAxis = LAST_X_AXIS;
            continue;
        }
        if (stepY)
        {
            voxel.y += step.y;
            tMax.y  += tDelta.y;
            lastStepAxis = LAST_Y_AXIS;
            continue;
        }
        voxel.z += step.z;
        tMax.z  += tDelta.z;
        lastStepAxis = LAST_Z_AXIS;
    }

    return false;
}

/*
* If we are outside of MAX_RENDER_DIST interpolate the color.
* Use the diffrence of distance and MAX_RENDER_DIST as the value that interpolates between colors.
* Clamp that value to HALF_MAX_RENDER_DIST (it creates that slow fading gradient).
* Divide the clamped value by HALF_MAX_RENDER_DIST to get number between 0. and 1. */
float4 FadeOutHorizont( float4 finalColor, float fDistance )
{
    return lerp( finalColor, 
                 BASE_SKY_COLOR,
                 clamp( fDistance - MAX_RENDER_DIST, 
                        0.f,
                        HALF_MAX_RENDER_DIST ) / HALF_MAX_RENDER_DIST );
}

// --------------------------------------------------------------------------------------------------------------------
[ RootSignature( "DescriptorTable( UAV( u0 ), SRV( t1, numDescriptors = 4 ), CBV( b1 ) )" ) ]
[ numthreads( 32, 8, 1 ) ]
void main(uint3 dispatchThreadId : SV_DispatchThreadID)
{
    uint outputImageWidth = 0, outputImageHeight = 0;
    g_OutputImage.GetDimensions( outputImageWidth, outputImageHeight );

    const float     scale           = tan( pc.fFov * 0.5 );
    const float     aspectRatio     = float( outputImageWidth ) / float( outputImageHeight );
    const float2    uv              = (( dispatchThreadId.xy + float2( 0.5, 0.5 )) / float2( outputImageWidth, outputImageHeight )) * 2. - 1.; 
    const float3    rd  = normalize( pc.CameraLookDir +
                                     uv.x * aspectRatio * scale * pc.CameraRight +
                                     uv.y * scale * pc.CameraUp );

    float4 finalColor = float4(1., 0.5, 1., 1.);

    float3    hitPos;
    uint      index;
    float     fDistance;
    float3    normal;
    int       hitType;
    float     reflectionPower;
    float     fRoughness;
    if ( !MarchTheRay( pc.CameraPos,
                       rd,
                       200,
                       hitPos,
                       index,
                       fDistance,
                       normal,
                       hitType ) ) 
    {
        g_OutputImage[ dispatchThreadId.xy ] = BASE_SKY_COLOR;
        return;
    }

    finalColor = float4( normal, 1. );

    if ( fDistance <= MAX_RENDER_DIST )
    {
        g_OutputImage[ dispatchThreadId.xy ] = finalColor;
        return;
    }

    g_OutputImage[ dispatchThreadId.xy ] = FadeOutHorizont( finalColor, fDistance );
}
