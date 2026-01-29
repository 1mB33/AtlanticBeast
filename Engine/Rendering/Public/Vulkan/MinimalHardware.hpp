#ifndef AB_MINIMAL_HARDWARE_H
#define AB_MINIMAL_HARDWARE_H

#include "WrapperHardware.hpp"
#include "Instance.hpp"

namespace B33::Rendering
{

class MinimalHardware : public HardwareWrapper
{
public: 

    BEAST_API MinimalHardware(::std::shared_ptr<const Instance> pInstance);

private:

    VkPhysicalDevice ChooseGPU(const ::std::shared_ptr<const Instance>& pInstance);

};

} // !B33::Rendering

#endif // !AB_MINIMAL_HARDWARE_H
