#define INF     1.f / 0.f
#define PI      3.141592
#define TWO_PI  PI * 2.
#define EPSILON 1e-4

// --------------------------------------------------------------------------------------------------------------------
float3x3 RotationMatrix( in const float3 angles )
{
    float sx = sin( angles.x );
    float cx = cos( angles.x );
    float sy = sin( angles.y );
    float cy = cos( angles.y );
    float sz = sin( angles.z );
    float cz = cos( angles.z );

    float3x3 rotX = float3x3( 1, 0, 0, 0, cx, sx, 0, -sx, cx );

    float3x3 rotY = float3x3( cy, 0, -sy, 0, 1, 0, sy, 0, cy );

    float3x3 rotZ = float3x3( cz, sz, 0, -sz, cz, 0, 0, 0, 1 );

    return mul( mul( rotX, rotY ), rotZ );
}
