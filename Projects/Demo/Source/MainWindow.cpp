#include "MainWindow.hpp"

#include "EntryPoints/DefaultSystemEntryPoint.hpp"


void MainWindow::Initialize( ::B33::System::ComponentBridge &bridge )
{
    m_WindowInstance.Create();
    m_WindowPuppet.BindToInput( m_WindowInstance.GetInput().lock() );
    m_WindowInstance.GetInput().lock()->StartCapturing();
}

void MainWindow::Update( ::B33::System::ComponentBridge &bridge, float fDelta )
{
    m_WindowInstance.Update( fDelta );
    m_WindowInstance.GetInput().lock()->Update( fDelta );
}

void MainWindow::Destroy( ::B33::System::ComponentBridge &bridge )
{
    m_WindowInstance.GetInput().lock()->StopCapturing();
    m_WindowInstance.Destroy();
}
