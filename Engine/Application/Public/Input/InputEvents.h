#ifndef AB_INPUT_EVENTS_H
#define AB_INPUT_EVENTS_H

#include "Bind.h"

typedef enum EAbInputEvents {
    AbKeyPress = 1,
    AbKeyRelease = AbKeyPress << 1,
    AbButtonPress = AbKeyRelease << 1,
    AbButtonRelease = AbButtonPress << 1,
    AbMotion = AbButtonRelease << 1,
} EAbInputEvents;

typedef struct AbInputStruct 
{
    EAbInputEvents Event;
    union {
        struct {
            AbKeyId KeyId;
        } Keyboard;

        struct {
            int32_t MouseX;
            int32_t MouseY;
        } Mouse;

        struct {
            AbKeyId KeyId;
        } MouseButton;
    };
} AbInputStruct;

#endif // !AB_INPUT_EVENTS_H

