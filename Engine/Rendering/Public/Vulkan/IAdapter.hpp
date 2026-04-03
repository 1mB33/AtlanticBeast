#ifndef B33_WRAPPER_H
#define B33_WRAPPER_H

#include "WrapperAdapter.hpp"

namespace B33::Rendering
{

template <class Derived>
class IAdapter : public AdapterWrapper
{
  public:
    IAdapter() = delete;

    IAdapter( ::std::shared_ptr<const HardwareWrapper> pHardware, const uint32_t uFlags )
      : AdapterWrapper( pHardware, uFlags, this )
    {
    }

    ~IAdapter() = default;

  public:
    IAdapter( IAdapter && )      = default;
    IAdapter( const IAdapter & ) = default;

    IAdapter &operator=( IAdapter && ) noexcept      = default;
    IAdapter &operator=( const IAdapter & ) noexcept = default;

  public:
    const ::std::vector<const char *> &GetExtensions() const
    {
        return static_cast<const Derived *>( this )->GetExtensionsImpl();
    }

    void *GetFeatures() const
    {
        return static_cast<const Derived *>( this )->GetFeaturesImpl();
    }
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_H
