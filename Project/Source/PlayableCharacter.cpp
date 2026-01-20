#include "MainCharacter.hpp"

void PlayablePaper::BindToInput(const ::std::shared_ptr<App::UserInput>& pInput)
{
    m_Controller.SignObject(pInput);

    AbInputBind ib;
    ib.Type = EAbBindType::Keyboard;
    ib.Keyboard = AbKeyboardBind{ EAbOnState::Continuous, App::AB_KEY_A };

    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveLeft, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_D;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveRight, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_W;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveFront, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_S;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveBack, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_Q;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveUp, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_E;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveDown, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_X;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionRemoveBlock, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_SPACE;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPlaceBlock, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_1;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPushBlockLowForce, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_2;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPushBlockMediumForce, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_3;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPushBlockHighForce, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_LEFTSHIFT;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionSprint, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_LEFTSHIFT;
    ib.Keyboard.KeyState = Release;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionWalk, nullptr, ib);

    ib.Type = Mouse; 
    pInput->Bind(m_Character.get(), &m_Controller, nullptr, &PaperController::UseActionMouse, ib);

    AbInputBind ibMouse;
    ibMouse.Type = EAbBindType::MouseButton;
    ibMouse.MouseButton.ButtonState = EAbOnState::Press;
    ibMouse.MouseButton.ButtonCode = App::AB_LEFT_MOUSE_BUTTON;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPlaceBlock, nullptr, ibMouse);

    ibMouse.MouseButton.ButtonCode = App::AB_RIGTH_MOUSE_BUTTON;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionRemoveBlock, nullptr, ibMouse);
}
