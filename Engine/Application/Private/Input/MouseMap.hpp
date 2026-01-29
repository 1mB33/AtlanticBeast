#ifndef AB_MOUSE_MAP_H
#define AB_MOUSE_MAP_H

#include "IBindMap.hpp"

namespace B33::App
{

class MouseMap : public IBindMap<MouseMap>
{

    struct DataForActionReplay
    {
        void*           pThis;
        AbMouseAction   Action;
    };

public:

    void BindActionImpl(const AbInputBind& ib, 
                        void* pThis,
                        AbAction a,
                        AbMouseAction ma);

    void UnbindActionImpl(const AbInputBind& ib, void* pThis);

public:

    void PlayAction(const float fDelta, int32_t fX, int32_t fY);

private:

    ::std::vector<DataForActionReplay> m_vMouseBinds;

};

} // !B33::App

#endif // !AB_MOUSE_MAP_H

