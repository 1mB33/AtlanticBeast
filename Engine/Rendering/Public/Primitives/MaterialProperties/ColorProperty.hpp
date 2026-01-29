#ifndef AB_COLOR_PROPERTY_H
#define AB_COLOR_PROPERTY_H

#include "B33Rendering.hpp"

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas(4) ColorProperty // : public IMaterialProperties
{
public:

    ColorProperty() 
        : m_uColor(0xFFFFFFFF)
    { }

public:

    void SetColor(::uint32_t uColor) 
    { m_uColor = (uColor & 0xFFFFFF00) | (m_uColor & 0x000000FF); }

    void SetAlpha(::uint32_t uAlpha) 
    { m_uColor = (uAlpha & 0x000000FF) | (m_uColor & 0xFFFFFF00); }

    void SetColorAndAlpha(::uint32_t uHexValue) 
    { m_uColor = uHexValue; }

private:

    ::uint32_t m_uColor;  

};

} // !B33::Rendering
#endif // !AB_COLOR_PROPERTY_H
