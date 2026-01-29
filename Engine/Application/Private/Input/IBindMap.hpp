#ifndef AB_BIND_MAP_H
#define AB_BIND_MAP_H

#include "Input/Bind.h"

namespace B33::App
{

template<class Map>
class IBindMap
{
public:

    IBindMap() = default;

    ~IBindMap() = default;

public:

    IBindMap(const IBindMap&) noexcept = default;
    IBindMap& operator=(const IBindMap&) noexcept = default;

    IBindMap(IBindMap&&) noexcept = default;
    IBindMap& operator=(IBindMap&&) noexcept = default;

public:

    void BindAction(const AbInputBind& ib, 
                    void* pThis,
                    AbAction a,
                    AbMouseAction ma) 
    { static_cast<Map*>(this)->BindActionImpl(ib, pThis, a, ma); }

    void UnbindAction(const AbInputBind& ib, void* pThis)
    { static_cast<Map*>(this)->UnbindActionImpl(ib, pThis); }

};

} // !Core
#endif // !AB_BIND_MAP_H
