#ifndef AB_COMPUTE_DEVICE_ADAPTER_H
#define AB_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace B33::Rendering
{

class ComputeAdapter : public ::B33::Rendering::AdapterWrapper 
                     , public ::B33::Rendering::IAdapter<::B33::Rendering::ComputeAdapter>
{
public:

    ComputeAdapter(::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> gpu)
        : ::B33::Rendering::AdapterWrapper(gpu, 
                                           VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT,
                                           GetExtensions(),
                                           GetFeaturesImpl())
        , m_pGPU(gpu)
    { }

public:

    const ::std::vector<const char*>& GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

private:

    ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> m_pGPU = nullptr;

};

} //!B33::Rendering
#endif // !AB_COMPUTE_DEVICE_ADAPTER_H
