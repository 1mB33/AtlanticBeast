#include "MainWindow.hpp"

#include "EntryPoints/DefaultSystemEntryPoint.hpp"

void MainWindow::Initialize( ::B33::System::ComponentBridge &bridge )
{
    m_WindowInstance.Create();
    m_WindowInstance.GetInput()->StartCapturing();
}

void MainWindow::Update( ::B33::System::ComponentBridge &bridge, float fDelta )
{
    m_WindowInstance.Update( fDelta );
    m_WindowInstance.GetInput()->Update( fDelta );
}

void MainWindow::Destroy( ::B33::System::ComponentBridge &bridge )
{
    m_WindowInstance.GetInput()->StopCapturing();
    m_WindowInstance.Destroy();
}
