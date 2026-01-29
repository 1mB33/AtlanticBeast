#ifndef AB_ROUGHNESS_PROPERTY_H
#define AB_ROUGHNESS_PROPERTY_H

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas(4) RoughnessProperty // : public IMaterialProperties
{
public:

    RoughnessProperty() 
        : m_fRoughness(0.08f)
    { }

public:

    void SetReflection(float fRef) 
    { 
        if (fRef >= 0.25f) {
            m_fRoughness = 0.25f;
            return;
        }
        m_fRoughness = fRef * 0.25f; 
    }

private:

    float m_fRoughness;  

};

} // !B33::Rendering
#endif // !AB_ROUGHNESS_PROPERTY_H
