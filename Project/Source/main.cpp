#include "Game.hpp"
#include "MainCharacter.hpp"

#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Raycaster/Renderer.hpp"
#include "Synchronization/DeltaTime.hpp"
#include "Synchronization/FpsLimiter.hpp"


using namespace std;
using namespace Core;
using namespace App;

int main()
{
    EmptyCanvas             renderWindow    = EmptyCanvas();
    const auto&             input           = renderWindow.GetInput();
    Voxels::Renderer        render          = { };
    ::std::shared_ptr<Game> g               = ::std::make_shared<Game>();
    DeltaTime               dt              = { };
    FpsLimiter              fl(1000.f / 120.f);
    float                   fFps            = 0.f;
    double                  dAvgSessionFps  = 0.;
    double                  dFrame          = 0.;

    ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();
	const auto& pc = pwc->GetCharacter();

    // Set up
	renderWindow.GetWindowDesc()->Name = L"Voxels Raytracing Demo!";
    renderWindow.Create();
    pwc->BindToInput(input);

    input->StartCapturing();

    render.SetCurrentCamera(::std::static_pointer_cast<Voxels::Camera>(pc));
    render.Initialize(renderWindow.GetWindowDesc(), g->GetWorld());
    
    pc->SetRotation(Voxels::Vec3 { -0.5f, 1.25f, 0.f });
    pc->SetPositon(Voxels::Vec3 { 14.5f, 2.25f, 25.f });
    pc->SetGrid(g); 

    g->Initialize();

	// Main loop
    dt.SetReferenceFrame();
    while (AppStatus::GetAppCurrentStatus()) 
    {   
        const float fDeltaMs = dt.FetchMs();
        renderWindow.Update(fDeltaMs);
        g->Update(fDeltaMs);
        render.Update(fDeltaMs);
        render.Render();
        const float fBlock = fl.Block(dt.DeltaMs(), fDeltaMs);

        // Skip the first 
        if (dFrame == 0) {
            dFrame = 1;
            continue;
        }

        fFps = 1000.f / fDeltaMs;

        if (dFrame <= INFINITY)
        {
            dAvgSessionFps = dAvgSessionFps * dFrame + fFps;
            dAvgSessionFps /= ++dFrame;
        }

        ::Core::Debug::Logger::Get().Log(::Core::Debug::Info, 
                                         L"AvgSessionFps: %lf Fps: %f Frame duration: %fms Blocked for: %fms",
                                         dAvgSessionFps,
                                         fFps,
                                         fDeltaMs,
                                         fBlock);
    }

    ::Core::Debug::Logger::Get().Log(::Core::Debug::Info, L"AvgFps: %lf Frames: %lf", dAvgSessionFps, dFrame);

    AB_LOG(Core::Debug::Info, L"App is closing...");

    render.Destroy();

    ::Core::Debug::Logger::Get().Flush();
}
