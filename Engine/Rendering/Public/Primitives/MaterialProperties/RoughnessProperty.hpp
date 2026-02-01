#ifndef AB_ROUGHNESS_PROPERTY_H
#define AB_ROUGHNESS_PROPERTY_H

namespace B33::Rendering
{

// FIXME: Some compilers add one byte in the empty IMaterialProperties class
class alignas(8) RoughnessProperty // : public IMaterialProperties
{
public:

    RoughnessProperty() 
        : m_fRoughness({ })
    { }

public:

    void SetReflection(float fRef, ::size_t uIndex) 
    { 
        if (fRef >= 0.25f) {
            m_fRoughness[uIndex] = 0.25f;
            return;
        }
        m_fRoughness[uIndex] = fRef * 0.25f; 
    }

private:

    ::std::vector<float> m_fRoughness;  

};

} // !B33::Rendering
#endif // !AB_ROUGHNESS_PROPERTY_H
