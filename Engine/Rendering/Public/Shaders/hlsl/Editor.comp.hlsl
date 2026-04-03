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


RWTexture2D<float4> g_OutputImage : register( u0 );

#if defined( VULKAN )

[[vk::push_constant]]
PushConstants pc;

#else

cbuffer PushConstantsBuffer : register( b1 )
{
    PushConstants pc;
};

#endif

[ RootSignature( "DescriptorTable( UAV( u0 ) ) " ) ][ numthreads( 32, 8, 1 ) ] void
main( uint3 dispatchThreadId : SV_DispatchThreadID )
{
    if (pc.uDebugMode != 1)
        return;

    uint outputImageWidth = 0, outputImageHeight = 0;
    g_OutputImage.GetDimensions( outputImageWidth, outputImageHeight );

    const float  scale       = tan( pc.fFov * 0.5 );
    const float  aspectRatio = float( outputImageWidth ) / float( outputImageHeight );
    const float2 uv =
        ( ( dispatchThreadId.xy + float2( 0.5, 0.5 ) ) / float2( outputImageWidth, outputImageHeight ) ) * 2. - 1.;
    const float3 rd =
        normalize( pc.CameraLookDir + uv.x * aspectRatio * scale * pc.CameraRight + uv.y * scale * pc.CameraUp );

    float4 finalColor = float4( 1., 0.5, 1., 1. );

    return;
}
