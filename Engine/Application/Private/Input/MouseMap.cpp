#include "B33Core.h"

#include "MouseMap.hpp"

namespace App
{

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::BindActionImpl(const AbInputBind& ib, void* pThis, AbAction a, AbMouseAction ma)
{
	AB_ASSERT(ib.Type == EAbBindType::Mouse);
    AB_ASSERT(a == nullptr);
    AB_ASSERT(pThis != nullptr);

	m_vMouseBinds.push_back(DataForActionReplay { pThis, ma });
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::UnbindActionImpl(const AbInputBind& ib, void* pThis)
{
	AB_ASSERT(ib.Type == EAbBindType::Mouse);
    AB_ASSERT(pThis != nullptr);

    auto it = m_vMouseBinds.begin();
    for (; it != m_vMouseBinds.end(); ++it) {
        if (it->pThis == pThis) {
            m_vMouseBinds.erase(it);
            return;
        }
    }

    AB_ASSERT(it != m_vMouseBinds.end());
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::PlayAction(const float fDelta, int32_t fX, int32_t fY)
{
	for (const auto& bind : m_vMouseBinds) {
		AB_ASSERT(bind.pThis);

		bind.Action(fDelta, bind.pThis, fX, fY);
    }
}

} // !App
