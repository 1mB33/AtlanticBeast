vec4 ExtractColorInt( in const uint uPackedColor )
{
    float r = float( ( uPackedColor >> 24 ) & 0xFFu ) / 255.;
    float g = float( ( uPackedColor >> 16 ) & 0xFFu ) / 255.;
    float b = float( ( uPackedColor >> 8 ) & 0xFFu ) / 255.;
    float a = float( uPackedColor & 0xFFu ) / 255.;

    return vec4( r, g, b, a );
}
