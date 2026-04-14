#ifndef B33_WRAPPER_ADAPTER_H
#define B33_WRAPPER_ADAPTER_H

#include "Vulkan/WrapperHardware.hpp"

namespace B33::Rendering
{

class AdapterWrapper
{
  public:
    AdapterWrapper()
      : m_pHardware( nullptr )
      , m_uFlags( -1 )
      , m_uQueueFamily()
      , m_Device()
      , m_Queue()
    {
    }

    ~AdapterWrapper()
    {
        B33_LOG( Core::Debug::Info, L"Destroying adapter" );
        if ( m_Device != VK_NULL_HANDLE )
        {
            vkDestroyDevice( m_Device, NULL );
            m_Device = VK_NULL_HANDLE;
        }
    }

  public:
    AdapterWrapper( AdapterWrapper && ) noexcept = default;
    AdapterWrapper( const AdapterWrapper & )     = delete;

    AdapterWrapper &operator=( AdapterWrapper && )      = delete;
    AdapterWrapper &operator=( const AdapterWrapper & ) = delete;

  public:
    uint32_t GetQueueFamilyIndex() const
    {
        return m_uQueueFamily;
    }

    VkDevice GetAdapterHandle() const
    {
        return m_Device;
    }

    VkQueue GetQueueHandle() const
    {
        return m_Queue;
    }

  public:
    template <class T>
    void Initialize( ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> pHardware, const T &adapter )
    {
        B33_LOG( Core::Debug::Info, L"Initializing adapter" );
        m_pHardware = pHardware;

        m_uFlags       = adapter.GetQueueFlags();
        m_uQueueFamily = ChooseQueueFamily( m_pHardware->GetPhysicalDevice(), m_uFlags );
        m_Device       = CreateDevice( m_pHardware->GetPhysicalDevice(),
                                       adapter.GetExtensions(),
                                       adapter.GetFeatures(),
                                       m_uQueueFamily );
        m_Queue        = CreateQueue( m_Device, m_uQueueFamily );
    }

  private:
    uint32_t ChooseQueueFamily( VkPhysicalDevice gpu, const uint32_t uFlags ) const;

    VkDevice CreateDevice( VkPhysicalDevice                 gpu,
                           const std::vector<const char *> &vExtensions,
                           const void                      *pFeatures,
                           const uint32_t                   uFamilyIndex ) const;

    VkQueue CreateQueue( VkDevice dv, uint32_t uQueueIndex ) const;

  private:
    ::std::shared_ptr<const HardwareWrapper> m_pHardware = nullptr;

    uint32_t m_uFlags       = 0;
    uint32_t m_uQueueFamily = 0;

    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue  m_Queue  = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_ADAPTER_H
