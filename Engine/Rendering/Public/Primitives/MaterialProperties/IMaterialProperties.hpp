#ifndef AB_IMATERIAL_PROPERTIES_H
#define AB_IMATERIAL_PROPERTIES_H

#include "B33Rendering.hpp"

namespace B33::Rendering
{

class alignas(4) IMaterialProperties
{
public:

    IMaterialProperties() = default;

    ~IMaterialProperties() = default;

public:

    IMaterialProperties(const IMaterialProperties&) noexcept = default;
    IMaterialProperties& operator=(const IMaterialProperties&) noexcept = default;

    IMaterialProperties(IMaterialProperties&&) noexcept = default;
    IMaterialProperties& operator=(IMaterialProperties&&) noexcept = default;

};

} // !B33::Rendering
#endif // !AB_IMATERIAL_PROPERTIES
