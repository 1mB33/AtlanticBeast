#if !defined( B33_IHARDWARE_HPP )
#    define B33_IHARDWARE_HPP

#    include "Instance.hpp"

namespace B33::Rendering
{

template <class DERIVED>
class IHardware
{
  public:
    IHardware() = default;

    ~IHardware() = default;

  public:
    IHardware( IHardware && ) noexcept            = default;
    IHardware &operator=( IHardware && ) noexcept = default;

    IHardware( const IHardware & ) noexcept            = default;
    IHardware &operator=( const IHardware & ) noexcept = default;

  public:
    ::VkPhysicalDevice ChooseHardware( const ::std::shared_ptr<const ::B33::Rendering::Instance> &pInstance ) const
    {
        return static_cast<const DERIVED *>( this )->ChooseHardwareImpl( pInstance );
    }
};

} // namespace B33::Rendering

#endif // !B33_IHARDWARE_HPP
