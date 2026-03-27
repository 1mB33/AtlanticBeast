#include "Colors.hlsl"
#include "Intersect.hlsl"
#include "Random.hlsl"

#define LAST_UNKNOWN_AXIS -1
#define LAST_X_AXIS       0
#define LAST_Y_AXIS       1
#define LAST_Z_AXIS       2

#define HIT_TYPE_UNKNOWN 0
#define HIT_TYPE_VOXEL   -1
#define HIT_TYPE_OBJECT  1

#define MAX_RENDER_DIST      28.f
#define HALF_MAX_RENDER_DIST 10.f
#define MAX_STEPS            73
#define BASE_SKY_COLOR       float4( .4078, .4725, 1., 1. )

#define PHONG_AMBIENT     0.2
#define PHONG_DIFFUSE     0.8
#define PHONG_SPECULAR    0.8
#define PHONG_SHININESS   512.0
#define PHONG_LIGHT_POS   float3( 20.0, 25.0, 10.0 )
#define PHONG_LIGHT_COLOR float3( 1., 1., .8 )

#define SOFT_SHADOW_R            0.5
#define SOFT_SHADOW_SAMPLES      8
#define SOFT_SHADOW_SAMPLE_STEP  ( TWO_PI / SOFT_SHADOW_SAMPLES )
#define SOFT_SHADOW_MIX_FACTOR   0.015
#define SOFT_SHADOW_SHADOW_CONST ( 1. / SOFT_SHADOW_SAMPLES )

struct PushConstants
{
    float3 CameraPos;
    uint   _Padding0;
    int3   GridSize;
    uint   _Padding1;
    float3 CameraLookDir;
    uint   _Padding2;
    float3 CameraRight;
    uint   _Padding3;
    float3 CameraUp;
    uint   _Padding4;
    float  fFov;
    uint   uDebugMode;
    uint   _Padding6;
    uint   _Padding7;
};

struct Voxel
{
    uint Type;
    uint Color;
    uint Id[ 26 ];
};

RWTexture2D<float4>      g_OutputImage : register( u0 );
StructuredBuffer<Voxel>  g_Voxels : register( t1 );
StructuredBuffer<float4> g_Positions : register( t2 );
StructuredBuffer<float4> g_Rotations : register( t3 );
StructuredBuffer<float4> g_HalfSizes : register( t4 );

#if defined( VULKAN )

[[vk::push_constant]]
PushConstants pc;

#else

cbuffer PushConstantsBuffer : register( b1 )
{
    PushConstants pc;
};

#endif

// --------------------------------------------------------------------------------------------------------------------
bool TestObjects( in const Voxel  onVoxel,
                  in const float3 ro,
                  in const float3 rd,
                  out uint        uHitIndex,
                  out float       distance,
                  out float3      hitCoords,
                  out float3      normal )
{
    distance = INF;

    uint   uLastId;
    float  fLastHitMin;
    float  fLastHitMax;
    float3 lastNormal;
    for ( uint k = 0; k < onVoxel.Type && k < 64; ++k )
    {
        uLastId = onVoxel.Id[ k ];

        // Object position is in oposite direction (more then 90 degrees)
        if ( dot( rd, g_Positions[ uLastId ].xyz - ro ) < 0. )
            continue;

        if ( !RayIntersectsAABB( ro,
                                 rd,
                                 g_Positions[ uLastId ].xyz,
                                 g_Rotations[ uLastId ].xyz,
                                 g_HalfSizes[ uLastId ].xyz,
                                 fLastHitMin,
                                 fLastHitMax,
                                 lastNormal ) )
        {
            continue;
        }

        if ( fLastHitMin < distance && fLastHitMin >= EPSILON && fLastHitMin < INF )
        {
            uHitIndex = uLastId;
            distance  = fLastHitMin;
            normal    = lastNormal;
        }
    }

    // Check if the hit was valid
    if ( distance == INF )
        return false;

    hitCoords = ro + rd * distance;
    return true;
}

// --------------------------------------------------------------------------------------------------------------------
bool MarchTheRay( in const float3 ro,
                  in const float3 rd,
                  in const int    maxSteps,
                  out float3      hitCoords,
                  out uint        hitIndex,
                  out float       distance,
                  out float3      normal,
                  out int         hitType )
{
    int3 voxel = int3( ro );
    int3 step  = int3( sign( rd ) );

    float3 tDelta = abs( 1.0 / rd );
    float3 tMax;
    bool3 tMin;

    // Calc initial offset
    float offset;
    int   i;
    for ( i = 0; i < 3; ++i )
    {
        offset    = rd[ i ] > 0.0 ? 1.0 - frac( ro[ i ] ) : frac( ro[ i ] );
        tMax[ i ] = tDelta[ i ] * offset;
    }

    int  index;
    uint testedVoxel;
    for ( i = 0; i < maxSteps; ++i )
    {
        if ( voxel.x <= 0 || voxel.x >= pc.GridSize.x || voxel.y <= 0 || voxel.y >= pc.GridSize.y || voxel.z <= 0 ||
             voxel.z >= pc.GridSize.z )
        {
            return false;
        }

        index = voxel.x + voxel.y * pc.GridSize.x + voxel.z * pc.GridSize.x * pc.GridSize.y;

        // Check for hits
        testedVoxel = g_Voxels[ index ].Type;

        if (testedVoxel > HIT_TYPE_UNKNOWN && testedVoxel <= uint( HIT_TYPE_VOXEL ) )
        {
            if ( pc.uDebugMode == 1 || testedVoxel == uint( HIT_TYPE_VOXEL ) )
            {
                distance  = dot(tMin, tMax - tDelta);
                hitIndex  = index;
                hitCoords = ro + rd * distance;
                hitType   = HIT_TYPE_VOXEL;
                normal    = -float3(step) * tMin;
                return true;
            }
            if ( TestObjects( g_Voxels[ index ], ro, rd, hitIndex, distance, hitCoords, normal ) )
            {
                hitType = HIT_TYPE_OBJECT;
                return true;
            }
        }

        tMin = bool3(
            ( tMax.x < tMax.y && tMax.x < tMax.z ),
            ( tMax.y < tMax.x && tMax.y < tMax.z ),
            ( tMax.z < tMax.y && tMax.z < tMax.x )
        );

        voxel += tMin * step;
        tMax  += tMin * tDelta;
    }

    return false;
}

// --------------------------------------------------------------------------------------------------------------------
float SoftShadowRay( in const float2 uv,
                     in const float3 from,
                     in const float3 to,
                     in const float3 normal,
                     in const int    maxDistance )
{
    float3 dir = normalize( to - from );
    float3 dummyHit;
    uint   dummyIndex;
    float3 dummyNormal;
    float  dummyDistance;
    int    dummyHitType;
    float  shadow = 1.;

    float  angle;
    float3 offset;
    for ( int i = 0; i < SOFT_SHADOW_SAMPLES; ++i )
    {
        angle  = float( i ) * SOFT_SHADOW_SAMPLE_STEP;
        offset = float3( cos( angle ), sin( angle ), 0. ) * SOFT_SHADOW_R;

        dir = normalize( to + offset - from );
        dir = lerp( dir, RandomPointOnHemisphere( normal, uv ), SOFT_SHADOW_MIX_FACTOR );

        if ( MarchTheRay( from + dir * EPSILON,
                          dir,
                          maxDistance,
                          dummyHit,
                          dummyIndex,
                          dummyDistance,
                          dummyNormal,
                          dummyHitType ) )
        {
            shadow -= SOFT_SHADOW_SHADOW_CONST;
        }
    }

    return shadow;
}

// --------------------------------------------------------------------------------------------------------------------
float3 PhongSoftShadows( in const float2 uv,
                         in const float3 camPos,
                         in const float3 hitPos,
                         in const float3 normal,
                         in const int    maxDistance )
{
    const float3 lightDir   = normalize( PHONG_LIGHT_POS - hitPos );
    const float3 viewDir    = normalize( camPos - hitPos );
    const float3 reflectDir = normalize( lightDir + viewDir );

    const float diff   = max( dot( normal, lightDir ), 0.0 );
    const float spec   = pow( max( dot( normal, reflectDir ), 0.0 ), PHONG_SHININESS );
    const float shadow = SoftShadowRay( uv, hitPos, PHONG_LIGHT_POS, normal, maxDistance );

    const float3 ambient  = PHONG_AMBIENT * PHONG_LIGHT_COLOR;
    const float3 diffuse  = PHONG_DIFFUSE * diff * PHONG_LIGHT_COLOR * shadow;
    const float3 specular = PHONG_SPECULAR * spec * PHONG_LIGHT_COLOR * shadow;

    return ambient + diffuse + specular;
}

// --------------------------------------------------------------------------------------------------------------------
float3 Reflection( in const float2 uv,
                   in float3       from,
                   in float3       to,
                   in int          maxSteps,
                   in const uint   bounces,
                   in const float3 baseColor,
                   in float3       normal,
                   in float        fMaterialReflectPower,
                   in float        fRoughness )
{
    float3 dir;
    float3 hit;
    uint   index;
    float  fDistance = 1.;
    float3 normalRef;
    int    hitType;

    float3 incident;
    float  fLocalReflect;
    float3 reflectedColor = baseColor;
    float4 hitBaseColor;

    for ( uint i = 0; i < bounces; ++i )
    {
        if ( fMaterialReflectPower <= 0.0 )
            break;

        dir = reflect( normalize( to - from ), normal );
        dir = lerp( dir, RandomPointOnHemisphere( normal, uv ), fRoughness * fDistance * 0.25 );

        if ( !MarchTheRay( to + dir * EPSILON, dir, maxSteps, hit, index, fDistance, normalRef, hitType ) )
        {
            reflectedColor = lerp( reflectedColor, BASE_SKY_COLOR.xyz, fMaterialReflectPower );
            return reflectedColor;
        }

        from   = to;
        to     = hit;
        normal = normalRef;

        if ( hitType == HIT_TYPE_VOXEL )
        {
            hitBaseColor  = ExtractColorInt( g_Voxels[ index ].Color );
            fLocalReflect = 0.1;
            fRoughness    = 0.01;
        }
        if ( hitType == HIT_TYPE_OBJECT )
        {
            hitBaseColor  = ExtractColorInt( 0xFFFFFFFF );
            fLocalReflect = 0.25;
            fRoughness    = 0.01;
        }

        reflectedColor = lerp( reflectedColor,
                               PhongSoftShadows( uv, from, hit, normal, maxSteps ) * hitBaseColor.xyz,
                               fMaterialReflectPower );

        fMaterialReflectPower *= fLocalReflect * 2;
        maxSteps = maxSteps * 0.5;
    }

    return reflectedColor;
}

/*
 * Use the diffrence of distance and MAX_RENDER_DIST as the value that interpolates between colors.
 * Clamp that value to HALF_MAX_RENDER_DIST (it creates that slow fading gradient).
 * Divide the clamped value by HALF_MAX_RENDER_DIST to get number between 0. and 1. */
float4 FadeOutHorizont( float4 finalColor, float distance )
{
    return lerp( finalColor,
                 BASE_SKY_COLOR,
                 clamp( distance - MAX_RENDER_DIST, 0.f, HALF_MAX_RENDER_DIST ) / HALF_MAX_RENDER_DIST );
}

// --------------------------------------------------------------------------------------------------------------------
[ RootSignature(
    "DescriptorTable( UAV( u0 ), SRV( t1, numDescriptors = 4 ), CBV( b1 ) )" ) ][ numthreads( 32, 8, 1 ) ] void
main( uint3 dispatchThreadId : SV_DispatchThreadID )
{
    uint outputImageWidth = 0, outputImageHeight = 0;
    g_OutputImage.GetDimensions( outputImageWidth, outputImageHeight );

    const float  scale       = tan( pc.fFov * 0.5 );
    const float  aspectRatio = float( outputImageWidth ) / float( outputImageHeight );
    const float2 uv =
        ( ( dispatchThreadId.xy + float2( 0.5, 0.5 ) ) / float2( outputImageWidth, outputImageHeight ) ) * 2. - 1.;
    const float3 rd =
        normalize( pc.CameraLookDir + uv.x * aspectRatio * scale * pc.CameraRight + uv.y * scale * pc.CameraUp );

    float4 finalColor = float4( 1., 0.5, 1., 1. );

    float3 hitPos;
    uint   index;
    float  hitDistance;
    float3 normal;
    int    hitType;
    float  reflectionPower;
    float  roughness;
    if ( !MarchTheRay( pc.CameraPos, rd, MAX_STEPS, hitPos, index, hitDistance, normal, hitType ) )
    {
        g_OutputImage[ dispatchThreadId.xy ] = BASE_SKY_COLOR;
        return;
    }

    if ( pc.uDebugMode == 1 )
    {
        finalColor = abs( float4( normal.xyz, 1.0 ) );
        finalColor.xyz =
            finalColor.xyz * PhongSoftShadows( dispatchThreadId.xy, pc.CameraPos, hitPos, normal, MAX_RENDER_DIST );

        g_OutputImage[ dispatchThreadId.xy ] = finalColor;
        return;
    }
    if ( hitType == HIT_TYPE_VOXEL )
    {
        finalColor      = ExtractColorInt( 0x00FF77FF );
        reflectionPower = 0.2;
        roughness       = 0.0;
    }
    if ( hitType == HIT_TYPE_OBJECT )
    {
        finalColor      = ExtractColorInt( 0xFFFFFFFF );
        reflectionPower = 0.45;
        roughness       = 0.0;
    }

    if ( hitDistance <= MAX_STEPS )
    {
        const int phongDistanceMax = int( distance( hitPos, PHONG_LIGHT_POS ) );
        finalColor.xyz =
            finalColor.xyz * PhongSoftShadows( dispatchThreadId.xy, pc.CameraPos, hitPos, normal, phongDistanceMax );

        finalColor.xyz = Reflection( dispatchThreadId.xy,
                                     pc.CameraPos,
                                     hitPos,
                                     MAX_RENDER_DIST,
                                     4,
                                     finalColor.xyz,
                                     normal,
                                     reflectionPower,
                                     roughness );
    }

    if ( hitDistance > MAX_RENDER_DIST )
        finalColor = FadeOutHorizont( finalColor, hitDistance );

    g_OutputImage[ dispatchThreadId.xy ] = finalColor;
}
