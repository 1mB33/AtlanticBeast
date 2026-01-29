#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "Window/WindowDesc.hpp"
#include "Window/WindowListener.hpp"
#include "Input/Bind.h"
#include "Input/KeyList.hpp"
#include "Input/MouseButtonList.hpp"

namespace B33::App
{

class ControllerObject;

class UserInput : public WindowListener
{
    
    struct BindHandle
    {
        AbInputBind Ib;
        void*       pThis;
    };

    struct UserInputImpl;

    using HandlesMap = ::std::unordered_map<void*, ::std::vector<BindHandle>>;
    using KeysStatus = ::std::bitset<AB_KEY_COUNT>;

public:

    BEAST_API explicit UserInput(::std::shared_ptr<WindowDesc> pWd = nullptr);

    BEAST_API ~UserInput();

public:

    /**
     * @brief Copied objects doesn't use the same capture status as the original.
     * Copies always are set to non capturing with empty keys status (all the keys are released).  
     */
    BEAST_API UserInput(const UserInput&) noexcept;

    /**
     * @brief Copied objects doesn't use the same capture status as the original.
     * Copies always are set to non capturing with empty keys status (all the keys are released).  
     */
    BEAST_API UserInput& operator=(const UserInput&) noexcept;

    /**
     * @brief Moved objects doesn't use the same capture status as the original.
     * Moved objects always are set to non capturing with empty keys status (all the keys are released).  
     */
    BEAST_API UserInput(UserInput&&) noexcept;

    /**
     * @brief Moved objects doesn't use the same capture status as the original.
     * Moved objects always are set to non capturing with empty keys status (all the keys are released).  
     */
    BEAST_API UserInput& operator=(UserInput&&) noexcept;

public:

    BEAST_API void StartCapturing();

    BEAST_API void StopCapturing();

    /**
     * @brief Binds an action or mouse action to a key or mouse.
     *
     * @param pThis - pointer to an object on which we are performing action
     * @param pCo - pointer to an object that controlls life time of pThis
     * @param action - action to be performed, should be null, if we are performing mouse action instead
     * @param mouseAction - mouse action to be performed, should be null, if we are performing action instead 
     * @param bind - AbInputBind struct describing the bind
     **/
    BEAST_API void Bind(void* pThis, 
                        ControllerObject* pCo,
                        AbAction action,
                        AbMouseAction mouseAction,
                        AbInputBind bind);

    BEAST_API void Unbind(ControllerObject* pCo);
    
    /**
     * Reads and consumes the input queue from WindowDesc.
     * Plays continues binds.
     */
    BEAST_API void Update(const float fDelta);

private:

    bool m_bIsCapturing;

    HandlesMap m_BindsHandles;
    KeysStatus m_vCurrentlyPressedKeys;

    ::std::unique_ptr<UserInputImpl> m_pImpl;

};

} // !B33::App
#endif // !AB_USER_INPUT_H
