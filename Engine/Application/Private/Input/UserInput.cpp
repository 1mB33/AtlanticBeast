#include "B33Core.h"

#include "Input/UserInput.hpp"

#include "Input/MouseButtonList.hpp"
#include "MouseMap.hpp"
#include "KeysMap.hpp"
#include "Input/InputEvents.h"
#include "Input/ControllerObject.hpp"

namespace B33::App
{

using namespace ::std;
using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

// --------------------------------------------------------------------------------------------------------------------
struct UserInput::UserInputImpl 
{
    KeysMap KeyReleaseMap;
    KeysMap KeyPressMap;
    KeysMap KeyContinuous;
    KeysMap ButtonReleaseMap = KeysMap(4);
    KeysMap ButtonPressMap = KeysMap(4);
    MouseMap MotionMouseMap;
};

// --------------------------------------------------------------------------------------------------------------------
UserInput::UserInput(::std::shared_ptr<WindowDesc> pWd)
    : WindowListener(pWd)
    , m_bIsCapturing(false)
    , m_BindsHandles()
    , m_vCurrentlyPressedKeys()
    , m_pImpl(make_unique<UserInputImpl>())
{ }

// --------------------------------------------------------------------------------------------------------------------
UserInput::~UserInput()
{ }

// --------------------------------------------------------------------------------------------------------------------
UserInput::UserInput(const UserInput& other) noexcept
    : WindowListener(other)
    , m_bIsCapturing(false)
    , m_BindsHandles(other.m_BindsHandles)
    , m_vCurrentlyPressedKeys()
    , m_pImpl(make_unique<UserInputImpl>(*other.m_pImpl.get()))
{ }

// --------------------------------------------------------------------------------------------------------------------
UserInput& UserInput::operator=(const UserInput& other) noexcept
{
    this->ListenToWindow(other.GetWindowDesc());
    this->m_bIsCapturing = false;
    this->m_BindsHandles = other.m_BindsHandles;
    this->m_vCurrentlyPressedKeys = { };
    this->m_pImpl = make_unique<UserInputImpl>(*other.m_pImpl.get());

    return *this;
}

// --------------------------------------------------------------------------------------------------------------------
UserInput::UserInput(UserInput&& other) noexcept
    : WindowListener(std::move(other))
    , m_bIsCapturing(false)
    , m_BindsHandles(std::move(other.m_BindsHandles))
    , m_vCurrentlyPressedKeys()
    , m_pImpl(std::move(other.m_pImpl))
{ }

// --------------------------------------------------------------------------------------------------------------------
UserInput& UserInput::operator=(UserInput&& other) noexcept
{
    this->ListenToWindow(::std::move(other.GetWindowDesc()));
    this->m_bIsCapturing = false;
    this->m_BindsHandles = ::std::move(other.m_BindsHandles);
    this->m_vCurrentlyPressedKeys = { };
    this->m_pImpl = ::std::move(other.m_pImpl);

    return *this;
}


// ---------------------------------------------------------------------------------------------------------------------
void UserInput::StartCapturing()
{ 
    if (!this->GetWindowDesc().get()) {
        Logger::Get().Log(Warning, L"UserInput::StartCapturing() -> Cannot start the capture, window desc is null.");
        Logger::Get().Log(Warning, L"UserInput::StartCapturing() -> Setting stop on the capture for safety.");
        StopCapturing();
        return;
    }

    if (m_bIsCapturing)
        Logger::Get().Log(Warning, L"UserInput::StartCapturing() -> Already capturing.");
        
    m_bIsCapturing = true;
}

// --------------------------------------------------------------------------------------------------------------------
void UserInput::StopCapturing()
{
    if (!m_bIsCapturing)
        Logger::Get().Log(Warning, L"UserInput::StopCapturing() -> Is NOT capturing already.");

    m_bIsCapturing = false; 
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update(const float fDelta)
{ 
    AB_ASSERT(this->GetWindowDesc().get() != nullptr);

    if (!m_bIsCapturing) {
        return;
    }

    const auto& pWindowDesc = this->GetWindowDesc();

    // Always replay the continuos keybinds that are currently pressed
    if (m_vCurrentlyPressedKeys.any()) {
        for (AbKeyId i = 0; i < AB_KEY_COUNT; ++i) {
            if (!m_vCurrentlyPressedKeys.test(i))
                continue;

            m_pImpl->KeyContinuous.PlayAction(fDelta, i);
        }
    }

    while (!pWindowDesc->InputStruct.empty()) 
    {
        AbInputStruct& is = pWindowDesc->InputStruct.front();

        switch (is.Event) 
        {
            case EAbInputEvents::AbKeyPress: {
                AbKeyId key = is.Keyboard.KeyId;

                if (key <= AB_INVALID_KEY || key >= AB_KEY_COUNT)
                    break;

                if (m_vCurrentlyPressedKeys.test(key))
                    break;

                m_vCurrentlyPressedKeys.set(key);

                m_pImpl->KeyPressMap.PlayAction(fDelta, key);
                // Don't wait for the next update to play continuos actions
                m_pImpl->KeyContinuous.PlayAction(fDelta, key);
                break;
            }

            case EAbInputEvents::AbKeyRelease: {
                AbKeyId key = is.Keyboard.KeyId;

                if (key <= AB_INVALID_KEY || key >= AB_KEY_COUNT)
                    break;

                if (!m_vCurrentlyPressedKeys.test(key))
                    break;

                m_vCurrentlyPressedKeys.flip(key);

                m_pImpl->KeyReleaseMap.PlayAction(fDelta, key);
                break;
            }

            case EAbInputEvents::AbButtonPress: {
                AbKeyId button = is.Keyboard.KeyId;

                if (button <= AB_INVALID_BUTTON || button >= AB_MOUSE_BUTTONS_COUNT)
                    break;

                m_pImpl->ButtonPressMap.PlayAction(fDelta, button);
                break;
            }

            case EAbInputEvents::AbButtonRelease: {
                AbKeyId button = is.Keyboard.KeyId;

                if (button <= AB_INVALID_BUTTON || button >= AB_MOUSE_BUTTONS_COUNT)
                    break;

                m_pImpl->ButtonReleaseMap.PlayAction(fDelta, button);
                break;
            }

            case EAbInputEvents::AbMotion: {
                m_pImpl->MotionMouseMap.PlayAction(fDelta, is.Mouse.MouseX, is.Mouse.MouseY);
                is.Mouse.MouseX = 0;
                is.Mouse.MouseY = 0;
                break;
            }
        }

		// Consume the input event
        pWindowDesc->InputStruct.pop();
    }

    pWindowDesc->LastEvent &= ~EAbWindowEvents::Input;
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Bind(void* pThis, ControllerObject* pCo, AbAction action, AbMouseAction mouseAction, AbInputBind bind)
{ 
    if (pCo->m_pUserInput.lock().get() != this) {
        Logger::Get().Log(Warning, L"Object must be signed by the UserInput, before binding.");
        Logger::Get().Log(Warning, L"Cannot bind the object.");
        return;
    }

    if (bind.Type & EAbBindType::Keyboard) 
    {
        if (bind.Keyboard.KeyCode <= AB_INVALID_KEY || bind.Keyboard.KeyCode >= AB_KEY_COUNT) {
            Logger::Get().Log(Error, L"Key code is an invalid code (code outside of boundries for keys).");
            Logger::Get().Log(Error, L"Can't bind the action for the keyboard.");
            Logger::Get().Log(Error, L"Action wasn't bound.");
            return;
        }

        switch (bind.Keyboard.KeyState)
        {
            case EAbOnState::Press:
                m_pImpl->KeyPressMap.BindAction(bind, pThis, action, nullptr);
                break;
            case EAbOnState::Release:
                m_pImpl->KeyReleaseMap.BindAction(bind, pThis, action, nullptr);
                break;
            case EAbOnState::Continuous:
                m_pImpl->KeyContinuous.BindAction(bind, pThis, action, nullptr);
                break;
            default:
                Logger::Get().Log(Error, 
                                  L"Key state wasn't valid? Coulnd't map the bind. [KeyState: %d]",
                                  bind.Keyboard.KeyState);
        }
    }
    else if (bind.Type & EAbBindType::MouseButton) {
        switch (bind.Keyboard.KeyState)
        {
            case EAbOnState::Press:
                m_pImpl->ButtonPressMap.BindAction(bind, pThis, action, nullptr);
                break;
            case EAbOnState::Release:
                m_pImpl->ButtonReleaseMap.BindAction(bind, pThis, action, nullptr);
                break;
            default:
                Logger::Get().Log(Error, 
                                  L"Button state wasn't valid? Coulnd't map the bind. [ButtonState: %d]",
                                  bind.Keyboard.KeyState);
        }
    }
    else if (bind.Type & EAbBindType::Mouse) {
        m_pImpl->MotionMouseMap.BindAction(bind, pThis, nullptr, mouseAction);
    }

    m_BindsHandles[pCo].push_back({ bind, pThis });
    AB_LOG(Info, L"New bind [Controller address: %p] [Bind type: %d]", pCo, bind.Type);
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Unbind(ControllerObject* pCo)
{
    const auto& handle = m_BindsHandles.find(pCo);
    
    if (handle == m_BindsHandles.end()) {
        Logger::Get().Log(Warning, 
                          L"Cannot unbind this bind from this UserInput, because UserInput doesn't handles it. "
                          "[Controller address: %p]", pCo);
        return;
    }
    AB_LOG(Info, L"Unbind [Controller address: %p]", pCo);

    for (const auto& bindHandle : handle->second) 
    {
        auto& bind = bindHandle.Ib;

        if (bind.Type & EAbBindType::Keyboard) {
            switch (bind.Keyboard.KeyState) 
            {
                case EAbOnState::Press:
                    m_pImpl->KeyPressMap.UnbindAction(bind, bindHandle.pThis);
                    break;
                case EAbOnState::Release:
                    m_pImpl->KeyReleaseMap.UnbindAction(bind, bindHandle.pThis);
                    break;
                case EAbOnState::Continuous:
                    m_pImpl->KeyContinuous.UnbindAction(bind, bindHandle.pThis);
                    break;
                default:
                    Logger::Get().Log(Error, 
                                      L"Key state wasn't valid? Coulnd't map the bind. [KeyState: %d]",
                                      bind.Keyboard.KeyState);
            }
        }
        else if (bind.Type & EAbBindType::Mouse) {
            m_pImpl->MotionMouseMap.UnbindAction(bind, bindHandle.pThis);
        }
    }

    m_BindsHandles.erase(handle);
}

} // !B33::App
