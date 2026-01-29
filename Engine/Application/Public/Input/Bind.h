#ifndef AB_KEY_BIND_H
#define AB_KEY_BIND_H

#include "B33Core.h"

/**
 * Empty struct to force binding type safety. Like a rock for your shoe.
 * */
typedef struct AbActionType {
    char Reserved;
} AbActionType;

typedef uint8_t AbKeyId;

typedef enum EState {
    IsPressed = 1,
    IsReleased = IsPressed << 1,
} EState;

typedef enum EAbOnState {
    Release = 1,
    Press = Release << 1,
    Continuous = Press << 1,
} EAbOnState;
 
typedef enum EAbBindType {
    Keyboard = 1,
    Mouse = Keyboard << 1,
    MouseButton = Mouse << 1,
} EAbBindType;

typedef struct AbKeyboardBind {
    EAbOnState KeyState;
    AbKeyId KeyCode;
} AbKeyboardBind;

typedef struct AbMouseButtonBind {
    EAbOnState ButtonState;
    AbKeyId ButtonCode;
} AbMouseButtonBind;

typedef struct AbInputBind {
    EAbBindType Type;

    union {
        AbKeyboardBind Keyboard;
        AbMouseButtonBind MouseButton;
    };
} AbInputBind;

typedef AbActionType (*AbAction)(const float fDelta, void* pThis);
typedef AbActionType (*AbMouseAction)(const float fDelta, void* pThis, int32_t X, int32_t y);
  
#endif // !AB_KEY_BIND_H
