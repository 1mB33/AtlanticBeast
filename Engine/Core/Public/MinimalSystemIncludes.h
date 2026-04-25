#ifndef B33_SYSTEMINCLU_H
#define B33_SYSTEMINCLU_H
#if defined( _WIN32 )
#    include "MinimalWindowsIncludes.h"
#elif defined( __linux__ )
#    include "MinimalLinuxIncludes.h"
#elif defined( __APPLE__ )
#    include "MinimalAppleIncludes.h"
#endif // !_WIN32
#endif // !B33_SYSTEMINCLU_H
