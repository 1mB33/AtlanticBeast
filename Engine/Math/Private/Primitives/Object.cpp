#include "B33Math.hpp"

#include "Primitives/Object.hpp"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
void WorldObject::SetPositon(Vec3 pos)
{ m_vPosition = pos; }

// ---------------------------------------------------------------------------------------------------------------------
void WorldObject::SetRotation(Vec3 rot)
{ m_vRotation = rot; }

// ---------------------------------------------------------------------------------------------------------------------
void WorldObject::AddPositon(const Vec3& pos)
{ m_vPosition += pos; }

// ---------------------------------------------------------------------------------------------------------------------
void WorldObject::AddRotation(const Rot3& rot)
{ m_vRotation += rot; }

} // !B33::Math
