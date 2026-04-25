#ifndef B33_RTX_HARDWARE_H
#define B33_RTX_HARDWARE_H

#include "Instance.hpp"
#include "Vulkan/IHardware.hpp"

namespace B33::Rendering
{

/**
 * Hardware wrapper that selects a GPU with features:
 * - rayTracingPipeline
 * - accelerationStructure
 * - bufferDeviceAddress
 **/
class RTXHardware : public ::B33::Rendering::IHardware<RTXHardware>
{
  public:
    BEAST_API RTXHardware();

  private:
    VkPhysicalDevice ChooseHardwareImpl( const ::std::shared_ptr<const ::B33::Rendering::Instance> &pInstance );
};

} // namespace B33::Rendering
#endif // !B33_RTX_HARDWARE_H
