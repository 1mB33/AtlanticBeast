#include "Include/Colors.hlsl"

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

cbuffer PushConstants : register( b1 )
{
    float3      CameraPos;
    uint        _Padding0;
    int3        GridSize;
    uint        _Padding1;
    float3      CameraLookDir;
    uint	    _Padding2;
    float3      CameraRight;
    uint        _Padding3;
    float3      CameraUp;
    uint        _Padding4;
    float       fFov;
    uint        uDebugMode;
    uint        _Padding6;
    uint        _Padding7;
};

[ RootSignature( "DescriptorTable( UAV( u0 ), SRV( t1, numDescriptors = 4 ), CBV( b1 ) )" ) ]
[ numthreads( 32, 8, 1 ) ]
void main()
{
    return;    
}

