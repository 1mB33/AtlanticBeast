#ifndef B33_COMPUTE_DEVICE_ADAPTER_H
#define B33_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"

namespace B33::Rendering
{

class GraphicsComputeAdapter : public ::B33::Rendering::IAdapter<::B33::Rendering::GraphicsComputeAdapter>
{
  public:
    GraphicsComputeAdapter()
      : IAdapter()
      , m_vExtensions()
    {
        m_vExtensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
    }

  public:
    const ::std::vector<const char *> &GetExtensionsImpl() const;

    void *GetFeaturesImpl() const;

    uint32_t GetQueueFlagsImpl() const
    {
        return ::VK_QUEUE_COMPUTE_BIT;
    }

  private:
    ::std::vector<const char *> m_vExtensions = {};
};

} // namespace B33::Rendering
#endif // !B33_COMPUTE_DEVICE_ADAPTER_H
