#ifndef AB_OBJECT_H
#define AB_OBJECT_H

#include "Math.hpp"

namespace B33::Math
{

/**
 * Holds positon of an object and rotation in radians.
 * */
class alignas(16) WorldObject
{
public:

	explicit WorldObject(Vec3 positon = Vec3 { 0.f, 0.f, 0.f },
						 Rot3 rotation = Vec3 { 0.f, 0.f, 0.f })
		: m_vPosition(Vec3())
		, m_vRotation(Rot3())
	{ }

    WorldObject(WorldObject&&) = default;
    WorldObject(const WorldObject&) = default;

    WorldObject& operator=(const WorldObject&) noexcept = default;
    WorldObject& operator=(WorldObject&&) noexcept = default;

    ~WorldObject() = default;

public:

	BEAST_API void SetPositon(Vec3 pos);

	BEAST_API void SetRotation(Rot3 rot);

	BEAST_API void AddPositon(const Vec3& pos);

	BEAST_API void AddRotation(const Rot3& rot);

public:

	const Vec3& GetPosition() const
	{ return m_vPosition; }

	const Rot3& GetRotation() const
	{ return m_vRotation; }

private:

	Vec3 m_vPosition;
	Rot3 m_vRotation;

};

} // !B33::Math

#endif // !AB_OBJECT_H
