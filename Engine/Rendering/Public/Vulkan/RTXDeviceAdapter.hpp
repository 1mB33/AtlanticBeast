#ifndef B33_DEVICE_ADAPTER_H
#define B33_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class RTXDeviceAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::RTXDeviceAdapter>
{
  public:
    RTXDeviceAdapter()
      : IAdapter()
    {
    }

  public:
    const ::std::vector<const char *> GetExtensionsImpl() const;

    void *GetFeaturesImpl() const;

    uint32_t GetQueueFlagsImpl() const
    {
        return ::VK_QUEUE_COMPUTE_BIT | ::VK_QUEUE_GRAPHICS_BIT;
    }
};

} // namespace B33::Rendering
#endif // !B33_DEVICE_ADAPTER_H
