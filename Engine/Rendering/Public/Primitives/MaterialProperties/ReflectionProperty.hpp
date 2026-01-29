#ifndef AB_REFLECTION_PROPERTY_H
#define AB_REFLECTION_PROPERTY_H

#include "B33Rendering.hpp"

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas(4) ReflectionProperty //: public IMaterialProperties
{
public:

    ReflectionProperty() 
        : m_fReflection(0.25f)
    { }

public:

    void SetReflection(float fRef) 
    { 
        if (fRef >= 0.5f) {
            m_fReflection = 0.5f;
            return;
        }
        m_fReflection = fRef * 0.5f; 
    }

private:

    float m_fReflection;  

};

} // !B33::Rendering
#endif // !AB_REFLECTION_PROPERTY_H
