#ifndef AB_COLORED_CUBE_AB
#define AB_COLORED_CUBE_AB

#include "Primitives/Cube.hpp"
#include "Primitives/MaterialProperties/ColorProperty.hpp"
#include "Primitives/MaterialProperties/ReflectionProperty.hpp"
#include "Primitives/MaterialProperties/RoughnessProperty.hpp"

namespace B33::Rendering
{

class alignas(16) ColoredCube : public B33::Math::Cube
                              , public B33::Rendering::ColorProperty
                              , public B33::Rendering::ReflectionProperty
                              , public B33::Rendering::RoughnessProperty
{ 
	uint32_t m_uPadding;
};

} // !B33::Rendering
#endif // !AB_COLORED_CUBE_AB
