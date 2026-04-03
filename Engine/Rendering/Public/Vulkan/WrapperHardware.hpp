#ifndef B33_WRAPPER_HARDWARE_H
#define B33_WRAPPER_HARDWARE_H

#include "Instance.hpp"

namespace B33::Rendering
{

class HardwareWrapper
{
  public:
    HardwareWrapper() = delete;

    explicit HardwareWrapper( ::std::shared_ptr<const B33::Rendering::Instance> pInstance,
                              VkPhysicalDevice                                  physicalDevice )
      : m_pInstance( pInstance )
      , m_PhysicalDevice( physicalDevice )
    {
        B33_LOG( Core::Debug::Info, L"Initializing hardware" );
    }

    ~HardwareWrapper()
    {
        B33_LOG( Core::Debug::Info, L"Destroying hardware" );
    }

  public:
    HardwareWrapper( HardwareWrapper && ) noexcept            = default;
    HardwareWrapper &operator=( HardwareWrapper && ) noexcept = default;

    HardwareWrapper( const HardwareWrapper & )            = delete;
    HardwareWrapper &operator=( const HardwareWrapper & ) = delete;

  public:
    VkPhysicalDevice GetPhysicalDevice() const
    {
        return m_PhysicalDevice;
    }

  protected:
    ::std::vector<VkPhysicalDevice> GetPhysicalDevices( VkInstance Instance );

  private:
    ::std::shared_ptr<const Instance> m_pInstance = nullptr;

    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
};

} // namespace B33::Rendering

#endif // !B33_WRAPPER_HARDWARE_H
