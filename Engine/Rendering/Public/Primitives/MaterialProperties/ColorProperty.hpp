#ifndef AB_COLOR_PROPERTY_H
#define AB_COLOR_PROPERTY_H

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas(8) ColorProperty // : public IMaterialProperties
{
public:

    ColorProperty() 
        : m_uColors({ })
    { }

public:

    void SetColor(::uint32_t uColor, ::size_t uIndex) 
    { m_uColors[uIndex] = (uColor & 0xFFFFFF00) | (m_uColors[uIndex] & 0x000000FF); }

    void SetAlpha(::uint32_t uAlpha, ::size_t uIndex) 
    { m_uColors[uIndex] = (uAlpha & 0x000000FF) | (m_uColors[uIndex] & 0xFFFFFF00); }

    void SetColorAndAlpha(::uint32_t uHexValue, ::size_t uIndex) 
    { m_uColors[uIndex] = uHexValue; }

private:

    ::std::vector<::uint32_t> m_uColors;

};

} // !B33::Rendering
#endif // !AB_COLOR_PROPERTY_H
