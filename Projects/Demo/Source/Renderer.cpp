#include "MyGame.hpp"
#include "Renderer.hpp"
#include "MainWindow.hpp"

using namespace B33;
using namespace B33::Math;

void Renderer::Initialize( ::B33::System::ComponentBridge &bridge )
{
    m_RendererInstance.SetCurrentCamera( bridge.QueryComponent<MyGame>().GetMainCharacter().GetCharacterHandle() );
    m_RendererInstance.Initialize( bridge.QueryComponent<MainWindow>().GetWindowInstance().GetWindowDesc() );
    m_RendererInstance.PushPipeline<::B33::Rendering::VoxelPipeline>(
        bridge.QueryComponent<MyGame>().GetGameInstance().GetWorld() );
}

void Renderer::Update( ::B33::System::ComponentBridge &bridge, float fDelta )
{
    auto                              &pc     = bridge.QueryComponent<MyGame>().GetMainCharacter().GetCharacterHandle();
    auto                              &g      = bridge.QueryComponent<MyGame>().GetGameInstance();
    const Vec3                         rot    = pc->GetRotation();
    const Vec3                         rotVec = Normalize( RotateY( RotateX( Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );
    const Vec3                         cameraRight = Normalize( Cross( rotVec, Vec3 { 0.f, -1.f, 0.f } ) );
    const Vec3                         cameraUp    = Cross( cameraRight, rotVec );
    B33::Rendering::VoxelPushConstants vpc         = {};
    vpc.fFov                                       = pc->GetFov() * AB_DEG_TO_RAD;
    vpc.CameraPos                                  = pc->GetPosition();
    vpc.CameraLookDir                              = rotVec;
    vpc.CameraRight                                = cameraRight;
    vpc.CameraUp                                   = cameraUp;
    const size_t uWorldWidth                       = g.GetWorld()->GetGridWidth();
    vpc.GridSize                                   = iVec3( uWorldWidth, uWorldWidth, uWorldWidth );
    vpc.uMode                                      = m_RendererInstance.GetDebugMode();

    m_RendererInstance.GetPipeline( 0 )->LoadPushConstants( vpc, sizeof( vpc ) );
    m_RendererInstance.Update( fDelta );
    m_RendererInstance.Render();
}

void Renderer::Destroy( ::B33::System::ComponentBridge &bridge )
{
    m_RendererInstance.Destroy();
}
