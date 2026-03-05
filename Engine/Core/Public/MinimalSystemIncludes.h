#ifndef AB_SYSTEMINCLU_H
#define AB_SYSTEMINCLU_H
#ifdef _WIN32
#    include "MinimalWindowsIncludes.h"
#elif __linux__
#    include "MinimalLinuxIncludes.h"
#endif // !_WIN32
#endif // !AB_SYSTEMINCLU_H
