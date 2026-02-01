#ifndef AB_OBJECTS_H
#define AB_OBJECTS_H

#include "B33Math.hpp"

namespace B33::Math
{

/**
 * Holds positon of an object and rotation in radians.
 * */
class WorldObjects
{
public:

	explicit WorldObjects()
		: m_uRollingIndex(0)
        , m_vPositions(64 * 64 * 64)
		, m_vRotations(64 * 64 * 64)
	{ }

    ~WorldObjects() = default;

public:

    WorldObjects(WorldObjects&&) = default;
    WorldObjects(const WorldObjects&) = default;

    WorldObjects& operator=(const WorldObjects&) noexcept = default;
    WorldObjects& operator=(WorldObjects&&) noexcept = default;

public:

	BEAST_API void SetPositon(Vec3 pos, ::size_t uIndex);

	BEAST_API void SetRotation(Rot3 rot, ::size_t uIndex);

	BEAST_API void AddPositon(const Vec3& pos, ::size_t uIndex);

	BEAST_API void AddRotation(const Rot3& rot, ::size_t uIndex);

public:

    const ::std::vector<Vec3>& GetPositions() const
    { return m_vPositions; }

    const ::std::vector<Vec3>& GetRotations() const
    { return m_vRotations; }

	const Vec3& GetPosition(::size_t uIndex) const
	{ return m_vPositions[uIndex]; }

	const Rot3& GetRotation(::size_t uIndex) const
	{ return m_vRotations[uIndex]; }

public:

    virtual ::size_t AddObject() 
    { 
        ::size_t i = m_uRollingIndex++; 

        m_vPositions[i] = Vec3();
        m_vRotations[i] = Vec3();

        return i;
    }

    virtual void RemoveObject(::size_t uIndex) 
    { }

private:

    ::size_t            m_uRollingIndex;
	::std::vector<Vec3> m_vPositions;
	::std::vector<Rot3> m_vRotations;

};

} // !B33::Math
#endif // !AB_OBJECTS_H
