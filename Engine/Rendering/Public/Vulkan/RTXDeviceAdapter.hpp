#ifndef B33_DEVICE_ADAPTER_H
#define B33_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class RTXDeviceAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::RTXDeviceAdapter>
{
  public:
    RTXDeviceAdapter()
      : IAdapter( ::VK_QUEUE_COMPUTE_BIT | ::VK_QUEUE_GRAPHICS_BIT )
    {
    }

  public:
    const ::std::vector<const char *> GetExtensionsImpl() const;

    void *GetFeaturesImpl() const;
};

} // namespace B33::Rendering
#endif // !B33_DEVICE_ADAPTER_H
