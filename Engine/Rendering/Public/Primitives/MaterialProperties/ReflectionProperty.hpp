#ifndef AB_REFLECTION_PROPERTY_H
#define AB_REFLECTION_PROPERTY_H

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas(8) ReflectionProperty //: public IMaterialProperties
{
public:

    ReflectionProperty() 
        : m_fReflections({ })
    { }

public:

    void SetReflection(float fRef, ::size_t uIndex) 
    { 
        if (fRef >= 0.5f) {
            m_fReflections[uIndex] = 0.5f;
            return;
        }
        m_fReflections[uIndex] = fRef * 0.5f; 
    }

private:

    ::std::vector<float> m_fReflections;  

};

} // !B33::Rendering
#endif // !AB_REFLECTION_PROPERTY_H
