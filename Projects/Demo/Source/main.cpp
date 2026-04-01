// #include "Game.hpp"
// #include "B33Core.h"
//
// #include "EmptyCanvas.hpp"
// #include "GameMaster.hpp"
// #include "MainCharacter.hpp"
// #include "Raycaster/PushConstants.hpp"
// #include "Vulkan/Renderer.hpp"
// #include "Raycaster/VoxelPipeline.hpp"
// #include "Synchronization/DeltaTime.hpp"
// #include "Synchronization/FpsLimiter.hpp"
//
// using namespace ::std;
// using namespace ::B33::App;
// using namespace ::B33::Core;
// using namespace ::B33::Core::Debug;
// using namespace ::B33::Math;
// using namespace ::B33::Rendering;
//
// #include "EntryPoints/DefaultSystemEntryPoint.hpp"
//
// int main()
// {
//     shared_ptr<EmptyCanvas<>> renderWindow = make_shared<EmptyCanvas<>>();
//     const auto               &input        = renderWindow->GetInput();
//     shared_ptr<Renderer>      render       = make_shared<Renderer>();
//     shared_ptr<Game>          g            = make_shared<Game>();
//     GameMasterPuppet          gameMaster   = GameMasterPuppet( renderWindow, render );
//     DeltaTime                 dt           = {};
//     DeltaTime                 dttime       = {};
//     FpsLimiter                fl( 1000.f / 120.f );
//     float                     fFps           = 0.f;
//     double                    dAvgSessionFps = 0.;
//     double                    dFrame         = 0.;
//     double                    dMin           = INFINITY;
//     double                    dMax           = -1.;
//
//     shared_ptr<PlayablePaper> pwc = make_shared<PlayablePaper>();
//     const auto               &pc  = pwc->GetCharacter();
//
//     // Set up
//
//     // Initialize Window Component
//     renderWindow->GetWindowDesc()->Name = L"Voxels Raytracing Demo!";
//     renderWindow->Create();
//
//     // Initialize Game Component
//     pwc->BindToInput( input );
//     gameMaster.BindToInput( input );
//     input->StartCapturing();
//     pc->SetRotation( Vec3 { -0.5f, 1.25f, 0.f } );
//     pc->SetPositon( Vec3 { 14.5f, 2.25f, 25.f } );
//     pc->SetGrid( g );
//     g->Initialize();
//
//     // Initialize Renderer Component
//     render->SetCurrentCamera( static_pointer_cast<Camera>( pc ) );
//     render->Initialize( renderWindow->GetWindowDesc() );
//     render->PushPipeline<VoxelPipeline>( g->GetWorld() );
//
//     // Main loop
//     dt.SetReferenceFrame();
//     while ( AppStatus::GetAppCurrentStatus() )
//     {
//         dttime.FetchMs();
//         const float fDeltaMs = dt.FetchMs();
//
//         renderWindow->Update( fDeltaMs );
//         const float fWindowTimeMs = dttime.FetchMs();
//
//         g->Update( fDeltaMs );
//         const float fGameTimeMs = dttime.FetchMs();
//
//         render->Update( fDeltaMs );
//         const Vec3         rot         = pc->GetRotation();
//         const Vec3         rotVec      = Normalize( RotateY( RotateX( Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );
//         const Vec3         cameraRight = Normalize( Cross( rotVec, Vec3 { 0.f, -1.f, 0.f } ) );
//         const Vec3         cameraUp    = Cross( cameraRight, rotVec );
//         VoxelPushConstants vpc         = {};
//         vpc.fFov                       = pc->GetFov() * AB_DEG_TO_RAD;
//         vpc.CameraPos                  = pc->GetPosition();
//         vpc.CameraLookDir              = rotVec;
//         vpc.CameraRight                = cameraRight;
//         vpc.CameraUp                   = cameraUp;
//         const size_t uWorldWidth       = g->GetWorld()->GetGridWidth();
//         vpc.GridSize                   = iVec3( uWorldWidth, uWorldWidth, uWorldWidth );
//         vpc.uMode                      = render->GetDebugMode();
//
//         render->GetPipeline( 0 )->LoadPushConstants( vpc, sizeof( vpc ) );
//         render->Render();
//
//
//         const float fRenderTimeMs = dttime.FetchMs();
//         const float fBlock        = fl.Block( dt.DeltaMs(), fDeltaMs );
//
//         // Skip the first
//         if ( dFrame == 0 )
//         {
//             dFrame = 1;
//             continue;
//         }
//
//         fFps = 1000.f / fDeltaMs;
//
//         if ( dFrame <= INFINITY )
//         {
//             dAvgSessionFps = dAvgSessionFps * dFrame + fFps;
//             dAvgSessionFps /= ++dFrame;
//         }
//
//         if ( dFrame >= 1000 )
//         {
//             if ( fFps < dMin )
//                 dMin = fFps;
//
//             if ( fFps > dMax && fFps <= ( 1000.f / fl.GetTarget() ) )
//                 dMax = fFps;
//         }
//
//         Logger::Get().Log( Info,
//                            L"Fps: %7.2f FpsMax: %7.2f  FpsMin: %7.2f AvgSessionFps: %7.2lf Frame duration: %7.2fms "
//                            L"Blocked for: %7.2fms WindowTime: %7.2fms GameTime: %7.2fms RenderTime: %7.2fms",
//                            fFps,
//                            dMax,
//                            dMin,
//                            dAvgSessionFps,
//                            fDeltaMs,
//                            fBlock,
//                            fWindowTimeMs,
//                            fGameTimeMs,
//                            fRenderTimeMs );
//     }
//
//     Logger::Get().Log( Info,
//                        L"Fps: %7.2f FpsMax: %7.2f  FpsMin: %7.2f AvgSessionFps: %7.2lf Frames: %lf",
//                        fFps,
//                        dMax,
//                        dMin,
//                        dAvgSessionFps,
//                        dFrame );
//
//     AB_LOG( Info, L"App is closing..." );
//
//     render->Destroy();
//
//     Logger::Get().Flush();
// }
