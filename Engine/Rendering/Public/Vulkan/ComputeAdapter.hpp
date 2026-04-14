#ifndef B33_COMPUTE_DEVICE_ADAPTER_H
#define B33_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class ComputeAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::ComputeAdapter>
{
  public:
    ComputeAdapter()
      : IAdapter( ::VK_QUEUE_COMPUTE_BIT )
      , m_vExtensions()
    {
        m_vExtensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
    }

  public:
    const ::std::vector<const char *> &GetExtensionsImpl() const;

    void *GetFeaturesImpl() const;

  private:
    ::std::vector<const char *> m_vExtensions = {};
};

} // namespace B33::Rendering
#endif // !B33_COMPUTE_DEVICE_ADAPTER_H
