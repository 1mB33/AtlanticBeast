#include "MyGame.hpp"
#include "MainWindow.hpp"

void MyGame::Initialize( ::B33::System::ComponentBridge &bridge )
{
    m_Game.Initialize();
    m_Paper.BindToInput( bridge.QueryComponent<MainWindow>().GetWindowInstance().GetInput() );
    m_Paper.GetCharacterHandle()->SetRotation( ::B33::Math::Vec3 { -0.5f, 1.25f, 0.f } );
    m_Paper.GetCharacterHandle()->SetPositon( ::B33::Math::Vec3 { 14.5f, 2.25f, 25.f } );
}

void MyGame::Update( ::B33::System::ComponentBridge &bridge, float fDelta )
{
    m_Game.Update( fDelta );
}

void MyGame::Destroy( ::B33::System::ComponentBridge &bridge ) {}
