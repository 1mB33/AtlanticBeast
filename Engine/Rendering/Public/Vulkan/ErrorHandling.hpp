#ifndef AB_ERROR_HANDLING_H
#define AB_ERROR_HANDLING_H

#define THROW_IF_FAILED( result )                                                                                      \
    {                                                                                                                  \
        if ( result != VK_SUCCESS )                                                                                    \
        {                                                                                                              \
            AB_LOG( Core::Debug::Error,                                                                                \
                    L"ThrowIfFailed, vulkan isn't working!!! Error code is: %d",                                       \
                    static_cast<int32_t>( result ) );                                                                  \
            throw AB_EXCEPT( "Ohh nooo... Vulkan isn't working!!!" );                                                  \
        }                                                                                                              \
    }

#endif // !AB_ERROR_HANDLING_H
