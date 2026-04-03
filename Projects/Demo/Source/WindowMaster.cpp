#include "WindowMaster.hpp"
#include "Input/UserInput.hpp"

void WindowMasterPuppet::BindToInput( const ::std::shared_ptr<B33::App::UserInput> &pInput )
{
    m_Controller.SignObject( pInput );

    AbInputBind ib;
    ib.Type     = EAbBindType::Keyboard;
    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::B33_KEY_Z };

    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::B33_KEY_F1 };
    pInput->Bind( &m_pGm, &m_Controller, &WindowMasterController::UseActionSetWindowMode, nullptr, ib );

    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::B33_KEY_F2 };
    pInput->Bind( &m_pGm, &m_Controller, &WindowMasterController::UseActionSetBorderless, nullptr, ib );

    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::B33_KEY_ESC };
    pInput->Bind( &m_pGm, &m_Controller, &WindowMasterController::UseActionExitGame, nullptr, ib );
}
