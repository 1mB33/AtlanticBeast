#ifndef B33_MINIMAL_HARDWARE_H
#define B33_MINIMAL_HARDWARE_H

#include "Instance.hpp"
#include "Vulkan/IHardware.hpp"

namespace B33::Rendering
{

class MinimalHardware : public ::B33::Rendering::IHardware<MinimalHardware>
{
  public:
    BEAST_API MinimalHardware();

  public:
    ::VkPhysicalDevice ChooseHardwareImpl( const ::std::shared_ptr<const ::B33::Rendering::Instance> &pInstance ) const;
};

} // namespace B33::Rendering
#endif // !B33_MINIMAL_HARDWARE_H
