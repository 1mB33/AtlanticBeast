#ifndef B33_WRAPPER_H
#define B33_WRAPPER_H

namespace B33::Rendering
{

template <class Derived>
class IAdapter
{
  public:
    IAdapter() = default;

    ~IAdapter() = default;

  public:
    IAdapter( IAdapter && ) noexcept      = default;
    IAdapter( const IAdapter & ) noexcept = default;

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

    uint32_t GetQueueFlags() const
    {
        return static_cast<const Derived *>( this )->GetQueueFlagsImpl();
    }
};

} // namespace B33::Rendering
#endif // !B33_WRAPPER_H
