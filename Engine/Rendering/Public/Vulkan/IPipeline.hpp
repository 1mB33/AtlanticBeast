#ifndef B33_IPIPELINE_H
#define B33_IPIPELINE_H

#include "Vulkan/IPushConstants.hpp"
#include "Vulkan/WrapperPipeline.hpp"

namespace B33::Rendering
{

template <class Derived>
class IPipeline : public PipelineWrapper
{
  public:
    IPipeline() = delete;

    IPipeline( VkPipelineStageFlagBits stage, VkPipelineBindPoint bindPoint )
      : PipelineWrapper( stage, bindPoint )
    {
    }

    ~IPipeline() = default;

  public:
    IPipeline( IPipeline && )      = default;
    IPipeline( const IPipeline & ) = default;

    IPipeline &operator=( IPipeline && ) noexcept      = default;
    IPipeline &operator=( const IPipeline & ) noexcept = default;

  public:
    ::size_t GetPushConstantsByteSize()
    {
        return static_cast<Derived *>( this )->GetPushConstantsByteSizeImpl();
    }

    IPushConstants *GetPushConstants()
    {
        return static_cast<Derived *>( this )->GetPushConstantsImpl();
    }

    ::VkDescriptorSetLayout CreateDescriptorLayout()
    {
        return static_cast<Derived *>( this )->CreateDescriptorLayoutImpl();
    }

    ::VkDescriptorPool CreateDescriptorPool()
    {
        return static_cast<Derived *>( this )->CreateDescriptorPoolImpl();
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

    template <class... RESOURCES_ARGS>
    void CreatePipelineResources( RESOURCES_ARGS... args )
    {
        return static_cast<Derived *>( this )->CreatePipelineResourcesImpl( ::std::forward<RESOURCES_ARGS>( args )... );
    }

  protected:
    using PipelineWrapper::GetAdaterInternal;

    using PipelineWrapper::GetShaderModuleInternal;

    using PipelineWrapper::GetDescriptorLayoutInternal;

    using PipelineWrapper::GetDescriptorPoolInternal;
};

} // namespace B33::Rendering
#endif // !B33_IPIPELINE_H
