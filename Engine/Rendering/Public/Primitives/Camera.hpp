#ifndef AB_CAMERA_H
#define AB_CAMERA_H

#include "Primitives/Object.hpp"

namespace B33::Rendering
{

/**
 * Basic camera.
 * Fov is horizontal, stored in degrees.
 * */
class Camera : public ::B33::Math::WorldObject
{
public:

    Camera(float fFov = 90.f)
		: m_fFov(fFov)
	{ }

    ~Camera() = default;

public:

    void IncreaseFov(float f)
    { m_fFov += f; }

    void SetFov(float f) 
    { m_fFov = f; }

public:

    float GetFov() 
    { return m_fFov; }

private:

	float m_fFov;

};

} // !B33::Rendering
#endif // !AB_CAMERA_H
