#pragma once

#include "Game.hpp"
#include "B33Rendering.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Input/KeyList.hpp"
#include "Primitives/Camera.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Raycaster/Rays.hpp"
#include "Raycaster/Renderer.hpp"
#include "Math.hpp"

class GameMaster
{
public:

    explicit GameMaster(std::shared_ptr<B33::Rendering::Renderer> pRenderer)
        : m_pRenderer(pRenderer)
    { }

public:

    void SwitchDebugMode(const float fDelta)
    { m_pRenderer->SetDebugMode(!m_pRenderer->GetDebugMode()); }

private:

    std::shared_ptr<B33::Rendering::Renderer> m_pRenderer;
};

class GameMasterController : public App::ControllerObject
{
public:

    GameMasterController() = default;

    ~GameMasterController() = default;

public:

    AB_DECL_ACTION(GameMaster, SwitchDebugMode, SwitchDebugMode);

};

class GameMasterPuppet
{
public:

    explicit GameMasterPuppet(std::shared_ptr<B33::Rendering::Renderer> pRenderer)
        : m_pGm(::std::make_shared<GameMaster>(pRenderer))
        , m_Controller()
    { }

public:

    void BindToInput(const ::std::shared_ptr<App::UserInput>& pInput);

    ::std::shared_ptr<GameMaster>& GetCharacter()
    { return m_pGm; }

private:

    ::std::shared_ptr<GameMaster> m_pGm;
    GameMasterController m_Controller;

};
