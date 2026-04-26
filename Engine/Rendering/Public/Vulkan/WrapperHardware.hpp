#ifndef B33_WRAPPER_HARDWARE_H
#define B33_WRAPPER_HARDWARE_H

#include "Instance.hpp"

namespace B33::Rendering
{

class HardwareWrapper
{
  public:
    HardwareWrapper()
      : m_pInstance( nullptr )
      , m_PhysicalDevice( nullptr )
    {
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

  public:
    template <class T>
    void Initialize( ::std::shared_ptr<const ::B33::Rendering::Instance> pInstance, const T &hardware )
    {
        B33_LOG( Core::Debug::Info, L"Initializing hardware" );
        m_pInstance      = pInstance;
        m_PhysicalDevice = hardware.ChooseHardware( pInstance );
    }

  private:
    ::std::shared_ptr<const Instance> m_pInstance = nullptr;

    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
};

} // namespace B33::Rendering

#endif // !B33_WRAPPER_HARDWARE_H
