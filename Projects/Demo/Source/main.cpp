#include "Game.hpp"
#include "GameMaster.hpp"
#include "MainCharacter.hpp"

#include "B33Core.h"
#include "EmptyCanvas.hpp"
#include "Raycaster/Renderer.hpp"
#include "Synchronization/DeltaTime.hpp"
#include "Synchronization/FpsLimiter.hpp"


using namespace ::std;
using namespace ::App;
using namespace ::B33::Core;
using namespace ::B33::Core::Debug;
using namespace ::B33::Math;
using namespace ::B33::Rendering;

int main()
{
    EmptyCanvas                                 renderWindow    = EmptyCanvas();
    const auto&                                 input           = renderWindow.GetInput();
    shared_ptr<Renderer>                        render          = make_shared<Renderer>();
    shared_ptr<Game>                            g               = make_shared<Game>();
    GameMasterPuppet                            gameMaster      = GameMasterPuppet(render);
    DeltaTime                                   dt              = { };
    DeltaTime                                   dttime          = { };
    FpsLimiter                                  fl(1000.f / 120.f);
    float                                       fFps            = 0.f;
    double                                      dAvgSessionFps  = 0.;
    double                                      dFrame          = 0.;

    shared_ptr<PlayablePaper> pwc = make_shared<PlayablePaper>();
	const auto& pc = pwc->GetCharacter();

    // Set up
	renderWindow.GetWindowDesc()->Name = L"Voxels Raytracing Demo!";
    renderWindow.Create();
    pwc->BindToInput(input);
    gameMaster.BindToInput(input);

    input->StartCapturing();

    render->SetCurrentCamera(static_pointer_cast<Camera>(pc));
    render->Initialize(renderWindow.GetWindowDesc(), g->GetWorld());
    
    pc->SetRotation(Vec3 { -0.5f, 1.25f, 0.f });
    pc->SetPositon(Vec3 { 14.5f, 2.25f, 25.f });
    pc->SetGrid(g); 

    g->Initialize();

	// Main loop
    dt.SetReferenceFrame();
    while (AppStatus::GetAppCurrentStatus()) 
    {   
        dttime.FetchMs();
        const float fDeltaMs = dt.FetchMs();
        renderWindow.Update(fDeltaMs);
        const float fWindowTimeMs = dttime.FetchMs();
        g->Update(fDeltaMs);
        const float fGameTimeMs = dttime.FetchMs();
        render->Update(fDeltaMs);
        render->Render();
        const float fRenderTimeMs = dttime.FetchMs();
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

        Logger::Get().Log(Info, 
                          L"AvgSessionFps: %7.2lf Fps: %7.2f Frame duration: %7.2fms Blocked for: %7.2fms WindowTime: %7.2fms GameTime: %7.2fms RenderTime: %7.2fms",
                          dAvgSessionFps,
                          fFps,
                          fDeltaMs,
                          fBlock,
                          fWindowTimeMs,
                          fGameTimeMs,
                          fRenderTimeMs);
    }

    Logger::Get().Log(Info, L"AvgFps: %lf Frames: %lf", dAvgSessionFps, dFrame);

    AB_LOG(Info, L"App is closing...");

    render->Destroy();

    Logger::Get().Flush();
}
