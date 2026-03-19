#ifndef AB_COMPUTE_DEVICE_ADAPTER_H
#define AB_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class ComputeAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::ComputeAdapter>
{
  public:
    ComputeAdapter( ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> pGpu )
      : IAdapter( pGpu, ::VK_QUEUE_COMPUTE_BIT )
    {
    }

  public:
    const ::std::vector<const char *> &GetExtensionsImpl() const;

    void *GetFeaturesImpl() const;
};

} // namespace B33::Rendering
#endif // !AB_COMPUTE_DEVICE_ADAPTER_H
