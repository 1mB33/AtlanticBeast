#include "Include/Colors.hlsl"

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


[ RootSignature( "DescriptorTable( UAV( u0 ), SRV( t1, numDescriptors = 4 ), CBV( b1 ) )" ) ]
[ numthreads( 32, 8, 1 ) ]
void main(uint3 dispatchThreadId : SV_DispatchThreadID)
{
    uint outputImageWidth = 0, outputImageHeight = 0;
    g_OutputImage.GetDimensions( outputImageWidth, outputImageHeight );

    const float     scale           = tan( pc.fFov * 0.5 );
    const float     aspectRatio     = float( outputImageWidth ) / float( outputImageHeight );
    const float2    uv              = (( dispatchThreadId.xy + float2( 0.5, 0.5 )) / float2( outputImageWidth, outputImageHeight )) * 2. - 1.; 
    const float3    ro  = pc.CameraPos;
    const float3    rd  = normalize( pc.CameraLookDir +
                                     uv.x * aspectRatio * scale * pc.CameraRight +
                                     uv.y * scale * pc.CameraUp );



    g_OutputImage[ dispatchThreadId.xy ] = float4( normalize( abs( pc.CameraPos ) ), 1. );
}

