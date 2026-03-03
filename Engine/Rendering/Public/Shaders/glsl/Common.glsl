// --------------------------------------------------------------------------------------------------------------------
bool IsCrosshair(in const vec2 uv, 
                 in const float fAspectRatio) 
{
    const float fCrossHairWidth = 0.0026;
    float fCrossHairWidthX = fCrossHairWidth;
    float fCrossHairWidthY = fCrossHairWidth;

    if (fAspectRatio >= 1.f) {
        fCrossHairWidthY *= fAspectRatio;
    } else {
        fCrossHairWidthX *= fAspectRatio;
    }

    return abs(uv).x < fCrossHairWidthX && abs(uv).y < fCrossHairWidthY;
}

