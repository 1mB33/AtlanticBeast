#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class RTXDeviceAdapter : public ::B33::Rendering::AdapterWrapper
                       , public ::B33::Rendering::IAdapter<::B33::Rendering::RTXDeviceAdapter>
{
public:

    RTXDeviceAdapter(::std::shared_ptr<const HardwareWrapper> pGpu)
        : AdapterWrapper(pGpu,
                         ::VK_QUEUE_COMPUTE_BIT | ::VK_QUEUE_GRAPHICS_BIT,
                         GetExtensionsImpl(),
                         GetFeaturesImpl())
    { }

public:

    const ::std::vector<const char*> GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

};

} // !B33::Rendering
#endif // !AB_DEVICE_ADAPTER_H
