// --------------------------------------------------------------------------------------------------------------------
float Random( in const vec2 uv )
{
    return fract( sin( dot( uv, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

// --------------------------------------------------------------------------------------------------------------------
float Random( in const vec2 uv, in const vec2 c )
{
    return fract( sin( dot( uv, c ) ) * 43758.5453 );
}

// --------------------------------------------------------------------------------------------------------------------
vec3 RandomPointOnHemisphere( in const vec3 normal )
{
    vec3 randPoint = vec3( Random( gl_GlobalInvocationID.xy - normal.yx ) - .5,
                           Random( gl_GlobalInvocationID.xy ) - .5,
                           Random( gl_GlobalInvocationID.yx + normal.xy ) - .5 );

    randPoint = normalize( randPoint );

    if ( dot( randPoint, normal ) < 0.0 )
        randPoint = -randPoint;

    return randPoint;
}

// --------------------------------------------------------------------------------------------------------------------
vec3 RandomPointOnHemisphere( in const vec3 normal, in const vec2 c )
{
    vec3 randPoint = vec3( Random( gl_GlobalInvocationID.xy - normal.yx, c ) - .5,
                           Random( gl_GlobalInvocationID.xy, c ) - .5,
                           Random( gl_GlobalInvocationID.yx + normal.xy, c ) - .5 );

    randPoint = normalize( randPoint );

    if ( dot( randPoint, normal ) < 0.0 )
        randPoint = -randPoint;

    return randPoint;
}

// --------------------------------------------------------------------------------------------------------------------
vec3 RandomColor()
{
    return normalize( vec3( Random( gl_GlobalInvocationID.xy ) - 123.5,
                            Random( gl_GlobalInvocationID.xy ) - .52234,
                            Random( gl_GlobalInvocationID.yx ) - 239809.5 ) );
}
