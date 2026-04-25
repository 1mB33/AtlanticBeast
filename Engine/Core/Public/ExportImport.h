#ifndef B33_EXPORT_IMPORT_H
#define B33_EXPORT_IMPORT_H
#if defined( __linux__ ) || defined( __APPLE__ )
#    ifdef _BEAST_EXPORTS
#        define BEAST_API __attribute__( ( visibility( "default" ) ) )
#    else
#        define BEAST_API
#    endif // !_BEAST_EXPORTS
#elif defined( _WIN32 )
#    ifdef _BEAST_EXPORTS
#        define BEAST_API __declspec( dllexport )
#    else
#        define BEAST_API __declspec( dllimport )
#    endif // !_BEAST_EXPORTS
#endif     // !__linux__
#endif     // !B33_EXPORT_IMPORT_
