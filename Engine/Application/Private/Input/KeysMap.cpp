#include "B33Core.h"

#include "KeysMap.hpp"

namespace App
{

// --------------------------------------------------------------------------------------------------------------------
KeysMap::KeysMap()
    : m_vKeys(AmountOfBindableKeys)
{
    memset(m_vKeys.data(), 0, sizeof(ActionReplayData) * m_vKeys.size());
}


// --------------------------------------------------------------------------------------------------------------------
KeysMap::KeysMap(size_t uAmountOfBindableKeys)
    : m_vKeys(uAmountOfBindableKeys)
{
    memset(m_vKeys.data(), 0, sizeof(ActionReplayData) * m_vKeys.size());
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::BindActionImpl(const AbInputBind& ib, void* pThis, AbAction a, AbMouseAction ma)
{
	AB_ASSERT(ib.Type == EAbBindType::Keyboard || ib.Type == EAbBindType::MouseButton);
    AB_ASSERT(ib.Keyboard.KeyCode > AB_INVALID_KEY && ib.Keyboard.KeyCode < AB_KEY_COUNT);
    AB_ASSERT(m_vKeys[ib.Keyboard.KeyCode].pThis == nullptr);
    AB_ASSERT(ma == nullptr);
    AB_ASSERT(pThis != nullptr);

	m_vKeys[ib.Keyboard.KeyCode] = ActionReplayData { pThis, a };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::UnbindActionImpl(const AbInputBind& ib, void* pThis)
{
	AB_ASSERT(ib.Type == EAbBindType::Keyboard || ib.Type == EAbBindType::MouseButton);
	AB_ASSERT(ib.Keyboard.KeyCode > AB_INVALID_KEY && ib.Keyboard.KeyCode < AB_KEY_COUNT);
    AB_ASSERT(pThis != nullptr);

	m_vKeys[ib.Keyboard.KeyCode].pThis = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::PlayAction(const float fDelta, AbKeyId keyCode)
{
	AB_ASSERT(keyCode > AB_INVALID_KEY && keyCode < AB_KEY_COUNT);

	const auto& playableAction = m_vKeys[keyCode];

    if (playableAction.pThis) {
	    playableAction.action(fDelta, playableAction.pThis);
    }
}

} // !App
