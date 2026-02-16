#include "Primitves.glsl"

const float INF     = 1.f / 0.f;
const float PI      = 3.141592;
const float TWO_PI  = PI * 2.;
const float EPSILON = 1e-4;

// --------------------------------------------------------------------------------------------------------------------
mat3 RotationMatrix(in const vec3 angles) 
{
    float sx = sin(angles.x);
    float cx = cos(angles.x);
    float sy = sin(angles.y);
    float cy = cos(angles.y);
    float sz = sin(angles.z);
    float cz = cos(angles.z);

    mat3 rotX = mat3(1,  0,   0,  
                     0, cx, -sx,  
                     0, sx,  cx);

    mat3 rotY = mat3( cy, 0, sy,  
                       0, 1,  0,
                     -sy, 0, cy);

    mat3 rotZ = mat3(cz, -sz, 0,
                     sz,  cz, 0,
                      0,   0, 1);

    return rotX * rotY * rotZ;
}

// --------------------------------------------------------------------------------------------------------------------
bool IntersectRayAABB(in const vec3 ro,
                      in const vec3 rd,
                      in const vec3 halfSize,
                      out float tMin, 
                      out float tMax)
{
    vec3 invDir = 1.0 / rd;
    vec3 t0s = (-halfSize - ro) * invDir;
    vec3 t1s = ( halfSize - ro) * invDir;

    vec3 tsmaller = min(t0s, t1s);
    vec3 tbigger  = max(t0s, t1s);

    tMin = max(max(tsmaller.x, tsmaller.y), tsmaller.z);
    tMax = min(min(tbigger.x, tbigger.y), tbigger.z);

    return tMax >= max(tMin, 0.0);
}

// --------------------------------------------------------------------------------------------------------------------
vec3 CubeNormal(in const vec3 lP, 
                in const vec3 halfSizes) 
{
    vec3 bias = abs(abs(lP) - halfSizes);

    if (bias.x < bias.y && bias.x < bias.z)
        return vec3(sign(lP.x), 0.0, 0.0);
    else if (bias.y < bias.x && bias.y < bias.z)
        return vec3(0.0, sign(lP.y), 0.0);
    else
        return vec3(0.0, 0.0, sign(lP.z));
}

// --------------------------------------------------------------------------------------------------------------------
bool RayIntersectsAABB(in const vec3 ro, 
                       in const vec3 rd,
                       in const Cube onCube, 
                       out float    fHitMin,
                       out float    fHitMax,
                       out vec3     normal)
{
    const mat3 cubeRot = RotationMatrix(onCube.Rot.xyz);
    const vec3 lro = cubeRot * (ro - onCube.Pos.xyz);
    const vec3 lrd = cubeRot * rd;

    if (!IntersectRayAABB(lro, 
                          lrd,
                          onCube.HalfSize.xyz,
                          fHitMin,
                          fHitMax)) 
    {
        return false;
    }

    const mat3 invCubeRot = inverse(cubeRot);
    normal = normalize(inverse(cubeRot) * CubeNormal(lro + lrd * fHitMin, onCube.HalfSize.xyz));

    return true;
}
