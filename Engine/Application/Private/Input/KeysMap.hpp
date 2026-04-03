#ifndef B33_KEYS_MAP_H
#define B33_KEYS_MAP_H

#include "IBindMap.hpp"
#include "Input/KeyList.hpp"

namespace B33::App
{

class KeysMap : public IBindMap<KeysMap>
{
    static constexpr size_t AmountOfBindableKeys = B33_KEY_COUNT;

    struct ActionReplayData
    {
        void    *pThis;
        AbAction action;
    };

  public:
    KeysMap();

    explicit KeysMap( size_t uAmountOfBindableKeys );

  public:
    void BindActionImpl( const AbInputBind &ib, void *pThis, AbAction a, AbMouseAction ma );

    void UnbindActionImpl( const AbInputBind &ib, void *pThis );

  public:
    void PlayAction( const float fDelta, AbKeyId keyCode );

  private:
    ::std::vector<ActionReplayData> m_vKeys;
};

} // namespace B33::App
#endif // !B33_KEYS_MAP_H
