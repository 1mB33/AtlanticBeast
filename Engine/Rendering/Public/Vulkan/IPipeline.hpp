#ifndef AB_IPIPELINE_H
#define AB_IPIPELINE_H

#include "Vulkan/WrapperPipeline.hpp"

namespace B33::Rendering
{

template <class Derived>
class IPipeline : public PipelineWrapper
{
  public:
    IPipeline() = delete;

    IPipeline( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da, const ::std::string &strPath )
      : PipelineWrapper( da, strPath, this )
    {
    }

    ~IPipeline() = default;

  public:
    IPipeline( IPipeline && )      = default;
    IPipeline( const IPipeline & ) = default;

    IPipeline &operator=( IPipeline && ) noexcept      = default;
    IPipeline &operator=( const IPipeline & ) noexcept = default;

  public:
    ::VkDescriptorSetLayout CreateDescriptorLayout()
    {
        return static_cast<Derived *>( this )->CreateDescriptorLayoutImpl();
    }

    ::VkDescriptorPool CreateDescriptorPool()
    {
        return static_cast<Derived *>( this )->CreateDescriptorPoolImpl();
    }

    ::VkShaderModule LoadShader( const ::std::string &strPath )
    {
        return static_cast<Derived *>( this )->LoadShaderImpl( strPath );
    }

    ::VkDescriptorSet CreateDescriptorSet()
    {
        return static_cast<Derived *>( this )->CreateDescriptorSetImpl();
    }

    ::VkPipelineLayout CreatePipelineLayout()
    {
        return static_cast<Derived *>( this )->CreatePipelineLayoutImpl();
    }

    ::VkPipeline CreatePipeline()
    {
        return static_cast<Derived *>( this )->CreatePipelineImpl();
    }

  protected:
    using PipelineWrapper::GetAdaterInternal;

    using PipelineWrapper::GetShaderModuleInternal;

    using PipelineWrapper::GetDescriptorLayoutInternal;

    using PipelineWrapper::GetDescriptorPoolInternal;
};

} // namespace B33::Rendering
#endif // !AB_IPIPELINE_H
