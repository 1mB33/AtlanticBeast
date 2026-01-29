#ifndef AB_KEYS_MAP_H
#define AB_KEYS_MAP_H

#include "IBindMap.hpp"
#include "Input/KeyList.hpp"

namespace B33::App
{

class KeysMap : public IBindMap<KeysMap>
{

    static constexpr size_t AmountOfBindableKeys = AB_KEY_COUNT;

    struct ActionReplayData
    {
        void* pThis;
        AbAction action;
    };

public:
    
    KeysMap();
    
    explicit KeysMap(size_t uAmountOfBindableKeys);

public:

    void BindActionImpl(const AbInputBind& ib,
                        void* pThis,
                        AbAction a, 
                        AbMouseAction ma);

    void UnbindActionImpl(const AbInputBind& ib, void* pThis);

public:

    void PlayAction(const float fDelta, AbKeyId keyCode);

private:

    ::std::vector<ActionReplayData> m_vKeys;

};

} // !B33::App
#endif // !AB_KEYS_MAP_H
