#ifndef AB_ENTRY_POINT_HPP
#define AB_ENTRY_POINT_HPP

#include "Synchronization/DeltaTime.hpp"
#include "../../EngineLoop.hpp"

int main( int argc, char *pArgv[] )
{
    ::B33::System::EngineLoop engineLoop = {};
    ::B33::Core::DeltaTime    dt         = {};

    engineLoop.InitializeComponents();

    dt.SetReferenceFrame();
    while ( ::B33::App::AppStatus::GetAppCurrentStatus() )
    {
        engineLoop.UpdateComponents( dt.FetchMs() );
    }


    engineLoop.DestroyComponents();
}

#endif // !AB_ENTRY_POINT_HPP
