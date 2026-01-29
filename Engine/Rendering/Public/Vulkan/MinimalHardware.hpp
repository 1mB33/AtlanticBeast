#ifndef AB_MINIMAL_HARDWARE_H
#define AB_MINIMAL_HARDWARE_H

#include "WrapperHardware.hpp"
#include "Instance.hpp"

namespace B33::Rendering
{

class MinimalHardware : public ::B33::Rendering::HardwareWrapper
{
public: 

    BEAST_API MinimalHardware(::std::shared_ptr<const ::B33::Rendering::Instance> pInstance);

private:

    ::VkPhysicalDevice ChooseGPU(const ::std::shared_ptr<const ::B33::Rendering::Instance>& pInstance);

};

} // !B33::Rendering
#endif // !AB_MINIMAL_HARDWARE_H
