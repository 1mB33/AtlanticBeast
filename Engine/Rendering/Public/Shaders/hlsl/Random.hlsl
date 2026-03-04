// --------------------------------------------------------------------------------------------------------------------
float Random(in const float2 s, in const float2 c)
{
    return frac( sin( dot( s, c )) * 43758.5453 );
}

// --------------------------------------------------------------------------------------------------------------------
float Random(in const float2 s)
{
    return Random( s, float2( 12.9898, 78.233 ) );
}

// --------------------------------------------------------------------------------------------------------------------
float3 RandomPointOnHemisphere(in const float3 normal, in const float2 s, in const float2 c)
{
    float3 randPoint = float3( Random( s.xy - normal.yx, c ) - .5,
                               Random( s.xy, c ) - .5, 
                               Random( s.yx + normal.xy, c ) - .5 );

    randPoint = normalize( randPoint );

    if ( dot( randPoint, normal ) < 0.0 )
        randPoint = -randPoint;

    return randPoint;
}

// --------------------------------------------------------------------------------------------------------------------
float3 RandomPointOnHemisphere(in const float3 normal, in const float2 s)
{
    return RandomPointOnHemisphere( normal, s, float2( 12.9898, 78.233 ) );
}

// --------------------------------------------------------------------------------------------------------------------
float3 RandomColor(in const float2 s)
{
    return normalize( float3(Random( s ) - 123.5,
                             Random( s ) - .52234, 
                             Random( s ) - 239809.5 ) );
}

