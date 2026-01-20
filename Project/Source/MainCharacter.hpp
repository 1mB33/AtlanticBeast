#pragma once

#include "Game.hpp"
#include "Voxels.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Input/KeyList.hpp"
#include "Primitives/Camera.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Raycaster/Rays.hpp"
#include "Math.hpp"

class PaperCharacter : public Voxels::Camera
{
public:

    template<class... U>
    explicit PaperCharacter(U&&... args)
        : m_g(nullptr)
        , Camera(::std::forward<U>(args)...)
        , m_fSpeed(m_fWalk)
    { }

public:

    void SetGrid(::std::shared_ptr<Game> vg)
    { m_g = vg; }

public:

    void PlaceBlock(const float)
    {
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.x), rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_g->GetWorld().get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit) {
            m_g->GenerateCube(Voxels::iVec3(hr.iHitCoords + hr.Normal));
        }
    }

    void RemoveBlock(const float)
    {
        Voxels::Vec3 rot        = this->GetRotation();
        Voxels::Vec3 lookDir    = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, 
                                                                                    rot.x), 
                                                                    rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_g->GetWorld().get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit) 
            m_g->RemoveCube(m_g->GetIdFromPos(hr.iHitCoords));
    }

    void Push(const float, const float fForceMul)
    {
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, 
                                                                                 rot.x), 
                                                                 rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_g->GetWorld().get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit) {
            m_g->PushCube(m_g->GetIdFromPos(hr.iHitCoords), hr.Normal, fForceMul);
        }
    }

    void MoveForwardBackwards(const float fDelta, float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.y);

        this->AddPositon(lookDir * fDir * (fDelta * m_fSpeed));
    }

    void Strafe(const float fDelta, float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.y + (90.f * Voxels::AB_DEG_TO_RAD));

        this->AddPositon(lookDir * fDir * (fDelta * m_fSpeed));
    }

    void MouseMove(const float, int32_t fX, int32_t fY)
    {
        this->AddRotation(Voxels::Rot3{ 0.00085f * fY, 0.00085f * fX, 0.f });
    }

    void Move(const float fDelta, const Voxels::Vec3& dir)
    {
        this->AddPositon(dir * (fDelta * m_fSpeed));
    }

    void ActivateSprint(const float) 
    {
        m_fSpeed = m_fSprint;
    }

    void ActivateWalk(const float) 
    {
        m_fSpeed = m_fWalk;
    }

private:

    ::std::shared_ptr<Game> m_g;

    uint32_t m_uColor;

    static constexpr float m_fWalk = 0.1;
    static constexpr float m_fSprint = 0.3;
    float m_fSpeed = -1.f;

};

class PaperController : public App::ControllerObject
{
public:

    PaperController() = default;

    ~PaperController() = default;

public:

    AB_DECL_ACTION(PaperCharacter, Strafe, MoveRight, 0.1f);

    AB_DECL_ACTION(PaperCharacter, Strafe, MoveLeft, -0.1f);

    AB_DECL_ACTION(PaperCharacter, MoveForwardBackwards, MoveFront, 0.1f);

    AB_DECL_ACTION(PaperCharacter, MoveForwardBackwards, MoveBack, -0.1f);

    AB_DECL_ACTION(PaperCharacter, Move, MoveUp, Voxels::Vec3{ 0.f, -0.1f, 0.f });

    AB_DECL_ACTION(PaperCharacter, Move, MoveDown, Voxels::Vec3{ 0.f, 0.1f, 0.f });

    AB_DECL_ACTION(PaperCharacter, PlaceBlock, PlaceBlock);

    AB_DECL_ACTION(PaperCharacter, RemoveBlock, RemoveBlock);

    AB_DECL_ACTION(PaperCharacter, Push, PushBlockLowForce, 10.f);

    AB_DECL_ACTION(PaperCharacter, Push, PushBlockMediumForce, 50.f);

    AB_DECL_ACTION(PaperCharacter, Push, PushBlockHighForce, 100.f);

    AB_DECL_ACTION(PaperCharacter, ActivateSprint, Sprint);

    AB_DECL_ACTION(PaperCharacter, ActivateWalk, Walk);

    AB_DECL_MOUSE_ACTION(PaperCharacter, MouseMove, Mouse);

};

class PlayablePaper
{
public:

    PlayablePaper()
        : m_Character(::std::make_shared<PaperCharacter>())
        , m_Controller()
    { }

    ~PlayablePaper() = default;

public:

    void BindToInput(const ::std::shared_ptr<App::UserInput>& pInput);

    ::std::shared_ptr<PaperCharacter>& GetCharacter()
    {
        return m_Character;
    }

private:

    ::std::shared_ptr<PaperCharacter> m_Character;
    PaperController m_Controller;

};
