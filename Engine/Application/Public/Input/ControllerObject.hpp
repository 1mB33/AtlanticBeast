#ifndef AB_CONTROLLER_OBJECT_H
#define AB_CONTROLLER_OBJECT_H

#include "B33Core.h"
#include "Input/UserInput.hpp"
#include "Bind.h"

namespace App
{

class ControllerObject
{

    friend UserInput;

public:

    ControllerObject() = default;

    ~ControllerObject()
    {
        if (auto pUserInput = m_pUserInput.lock())
            pUserInput->Unbind(this);
    }

public:

    ControllerObject(const ControllerObject&) noexcept = delete;
    ControllerObject& operator=(const ControllerObject&) noexcept = delete;

    ControllerObject(ControllerObject&& other) noexcept
        : m_pUserInput(std::move(other.m_pUserInput))
    { }

public:

    void SignObject(::std::weak_ptr<UserInput> pUserInput)
    { 
        // We can be signed by only one UserInput
        if (!m_pUserInput.expired()) {
            AB_ASSERT(m_pUserInput.lock().get() == pUserInput.lock().get());
            AB_LOG(Core::Debug::Warning, L"ControllerObject can be signed only by one UserInput.");
            return;
        }

        m_pUserInput = pUserInput;
    }

private:

    ::std::weak_ptr<UserInput> m_pUserInput;

};

} // !App



#define AB_DECL_ACTION(baseClass, action, customName, ...)                          \
    static ::AbActionType UseAction##customName(const float fDelta, void* pThis)    \
    {                                                                               \
        AB_ASSERT(pThis != nullptr);                                                \
                                                                                    \
        static_cast<baseClass*>(pThis)->action(fDelta AB_VA_ARGS_(__VA_ARGS__));    \
        return ::AbActionType();                                                    \
    }



#define AB_DECL_MOUSE_ACTION(baseClass, action, customName)                                                 \
    static ::AbActionType UseAction##customName(const float fDelta, void* pThis, int32_t fX, int32_t fY)    \
    {                                                                                                       \
        AB_ASSERT(pThis != nullptr);                                                                        \
                                                                                                            \
        static_cast<baseClass*>(pThis)->action(fDelta, fX, fY);                                             \
        return ::AbActionType();                                                                            \
    }



#endif // !AB_CONTROLLER_OBJECT_H

