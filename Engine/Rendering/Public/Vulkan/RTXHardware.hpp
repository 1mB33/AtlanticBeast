#ifndef AB_RTX_HARDWARE_H
#define AB_RTX_HARDWARE_H

#include "Instance.hpp"
#include "WrapperHardware.hpp"

namespace B33::Rendering
{

/**
* Hardware wrapper that selects a GPU with features:
* - rayTracingPipeline
* - accelerationStructure
* - bufferDeviceAddress
**/
class RTXHardware : public HardwareWrapper
{
public:

    BEAST_API RTXHardware(::std::shared_ptr<const Instance> pInstance);

private:
    
    VkPhysicalDevice ChooseGPU(const ::std::shared_ptr<const Instance>& pInstance);

};

} // !B33::Rendering

#endif // !AB_RTX_HARDWARE_H
