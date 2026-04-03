#if defined( __linux__ )
#    ifndef B33_ENTRY_POINT_HPP
#        define B33_ENTRY_POINT_HPP

#        include "../../B33System.hpp"
#        include "Synchronization/DeltaTime.hpp"
#        include "../../EngineLoop.hpp"

int main( int argc, char *pArgv[] )
{
    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Starting B33..." );
    ::B33::System::EngineLoop engineLoop = {};
    ::B33::Core::DeltaTime    dt         = {};


    engineLoop.InitializeComponents();

    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"B33 started..." );
    dt.SetReferenceFrame();
    while ( ::B33::App::AppStatus::GetAppCurrentStatus() )
    {
        engineLoop.UpdateComponents( dt.FetchMs() );
    }

    engineLoop.DestroyComponents();


    ::B33::Core::Debug::Logger::Get().Log( ::B33::Core::Debug::Info, L"Closing B33..." );
    ::B33::Core::Debug::Logger::Get().Flush();
}

#    endif // !B33_ENTRY_POINT_HPP
#endif     // !__linux__
