#include "B33Math.hpp"

#include "Primitives/Objects.hpp"

namespace B33::Math
{

// ---------------------------------------------------------------------------------------------------------------------
void WorldObjects::SetPositon(Vec3 pos, size_t uIndex)
{ m_vPositions[uIndex] = pos; }

// ---------------------------------------------------------------------------------------------------------------------
void WorldObjects::SetRotation(Vec3 rot, size_t uIndex)
{ m_vRotations[uIndex] = rot; }

// ---------------------------------------------------------------------------------------------------------------------
void WorldObjects::AddPositon(const Vec3& pos, size_t uIndex)
{ m_vPositions[uIndex] += pos; }

// ---------------------------------------------------------------------------------------------------------------------
void WorldObjects::AddRotation(const Rot3& rot, size_t uIndex)
{ m_vRotations[uIndex] += rot; }

} // !B33::Math
