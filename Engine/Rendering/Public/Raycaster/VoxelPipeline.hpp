#ifndef AB_PIPELINE_H
#define AB_PIPELINE_H

#include "ExportImport.h"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/IPipeline.hpp"
#include "Vulkan/Memory.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/PushConstants.hpp"
#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

class VoxelPipeline : public IPipeline<VoxelPipeline>
{
    using Vec  = ::B33::Math::Vec3;
    using iVec = ::B33::Math::iVec3;

  public:
    enum EShaderResource
    {
        VoxelGrid       = 1,
        ObjectPositions = VoxelGrid + 1,
        ObjectRotations = ObjectPositions + 1,
        ObjectHalfSizes = ObjectRotations + 1,
    };

  public:
    VoxelPipeline( ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> da,
                   ::std::shared_ptr<::B33::Rendering::Memory>               mem,
                   ::std::shared_ptr<const ::WindowDesc>                     win )
      : IPipeline( da, ::B33::App::AppResources::Get().GetExecutablePathA() + "/Assets/Shaders/Raycast.spv" )
      , m_pMemory( mem )
      , m_pWindowDesc( win )
      , m_pVoxelGrid( nullptr )
    {
        AB_LOG( Core::Debug::Info, L"Creating a pipeline!" );
    }

    BEAST_API ~VoxelPipeline();

  public:
    BEAST_API virtual void Update() override final;

    BEAST_API virtual void RecordCommands( VkCommandBuffer &cmdBuffer ) override final;

    BEAST_API virtual void Reset() override final;

    BEAST_API virtual void LoadImage( VkImage image ) override final;

    virtual void LoadPushConstants( float      fFov,
                                    Vec        cameraPos,
                                    Vec        cameraLookDir,
                                    Vec        cameraRight,
                                    Vec        cameraUp,
                                    ::uint32_t uDebugMode ) override final
    {
        ::uint32_t uGridWidth = m_pVoxelGrid->GetGridWidth();

        m_Vpc.CameraPos     = cameraPos;
        m_Vpc.CameraLookDir = cameraLookDir;
        m_Vpc.CameraRight   = cameraRight;
        m_Vpc.CameraUp      = cameraUp;
        m_Vpc.fFov          = fFov;
        m_Vpc.GridSize      = iVec( uGridWidth, uGridWidth, uGridWidth );
        m_Vpc.uMode         = uDebugMode;
    }

    void CreatePipelineResourcesImpl( ::std::shared_ptr<::B33::Rendering::CubeWorld> pWorld );

    ::VkDescriptorSet CreateDescriptorSetImpl();

    ::VkPipelineLayout CreatePipelineLayoutImpl();

    ::VkPipeline CreatePipelineImpl();

    ::VkDescriptorSetLayout CreateDescriptorLayoutImpl();

    ::VkDescriptorPool CreateDescriptorPoolImpl();

    ::VkShaderModule LoadShaderImpl( const ::std::string &strPath );

  private:
    UploadDescriptor GetUniformUploadDescriptor( const ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> &outBuffer,
                                                 const EShaderResource                                      &sr );

  private:
    ::std::shared_ptr<::B33::Rendering::Memory>          m_pMemory     = nullptr;
    ::std::shared_ptr<const ::WindowDesc>                m_pWindowDesc = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::Swapchain> m_pSwapChain  = nullptr;

    ::B33::Rendering::VoxelPushConstants m_Vpc;


    // Assets memory
    ::std::shared_ptr<::B33::Rendering::IWorldGrid>      m_pVoxelGrid = nullptr;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_VoxelBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_PositionsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_RotationsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUBuffer>       m_HalfSizesBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageVoxelBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StagePositonsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageRotationsBuffer;
    ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> m_StageHalfSizesBuffer;

    ::uint32_t m_uStorageBuffersFlags;


    ::VkImageView m_ImageView = VK_NULL_HANDLE;
};

} // namespace B33::Rendering
#endif // !AB_PIPELINE_H
