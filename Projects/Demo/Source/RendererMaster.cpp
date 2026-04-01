#include "RendererMaster.hpp"
#include "Input/UserInput.hpp"

void RendererMasterPuppet::BindToInput( const ::std::shared_ptr<B33::App::UserInput> &pInput )
{
    m_Controller.SignObject( pInput );

    AbInputBind ib;
    ib.Type     = EAbBindType::Keyboard;
    ib.Keyboard = AbKeyboardBind { EAbOnState::Press, B33::App::AB_KEY_Z };
    pInput->Bind( &m_Master, &m_Controller, &RendererMasterController::UseActionSetWindowMode, nullptr, ib );
}
