#ifndef AB_COLORED_CUBES_AB
#define AB_COLORED_CUBES_AB

#include "Primitives/Cubes.hpp"
#include "Primitives/MaterialProperties/ColorProperty.hpp"
#include "Primitives/MaterialProperties/ReflectionProperty.hpp"
#include "Primitives/MaterialProperties/RoughnessProperty.hpp"

namespace B33::Rendering
{

class ColoredCubes : public B33::Math::Cubes
                   , public B33::Rendering::ColorProperty
                   , public B33::Rendering::ReflectionProperty
                   , public B33::Rendering::RoughnessProperty
{ };

} // !B33::Rendering
#endif // !AB_COLORED_CUBES_AB
