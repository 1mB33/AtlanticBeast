#include "GameMaster.hpp"

void GameMasterPuppet::BindToInput(const ::std::shared_ptr<App::UserInput>& pInput)
{
    m_Controller.SignObject(pInput);

    AbInputBind ib;
    ib.Type = EAbBindType::Keyboard;
    ib.Keyboard = AbKeyboardBind{ EAbOnState::Press, App::AB_KEY_Z };

    pInput->Bind(m_pGm.get(), &m_Controller, &GameMasterController::UseActionSwitchDebugMode, nullptr, ib);
}

