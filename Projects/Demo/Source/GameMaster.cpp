#include "GameMaster.hpp"

void GameMasterPuppet::BindToInput( const ::std::shared_ptr<B33::App::UserInput> &pInput )
{
    m_Controller.SignObject( pInput );

    AbInputBind ib;
    ib.Type     = EAbBindType::Keyboard;
    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::AB_KEY_Z };

    pInput->Bind( m_pGm.get(), &m_Controller, &GameMasterController::UseActionSwitchDebugMode, nullptr, ib );

    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::AB_KEY_F1 };
    pInput->Bind( m_pGm.get(), &m_Controller, &GameMasterController::UseActionSetWindowMode, nullptr, ib );

    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::AB_KEY_F2 };
    pInput->Bind( m_pGm.get(), &m_Controller, &GameMasterController::UseActionSetBorderless, nullptr, ib );

    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::AB_KEY_ESC };
    pInput->Bind( m_pGm.get(), &m_Controller, &GameMasterController::UseActionExitGame, nullptr, ib );
}
