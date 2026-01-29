#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"
#include "Input/UserInput.hpp"

namespace B33::App
{

/**
 * Basic empty window implementation for rendering and game. Has it's own built in UserInput instance.
 *
 * @param bManualInputUpdate - if true, input update will not be called automatically on every window update,
 * this way you can manualy update the input on another thread or at a different time.
 * @param GamePolicy - policy that defines window behaviour on different os level events. Defaults to DefaultGameSystemWindowPolicy
 * */
template<bool bManualInputUpdate = false, class GamePolicy = DefaultGameSystemWindowPolicy>
class EmptyCanvas : public ::B33::App::IBaseWindow<EmptyCanvas<bManualInputUpdate>, GamePolicy>
{

	friend class IBaseWindow<EmptyCanvas<bManualInputUpdate>, GamePolicy>;

public:

    explicit EmptyCanvas(std::wstring wstrWindowName = L"EmptyCanvas")
        : IBaseWindow<EmptyCanvas, GamePolicy>(
                CreateWindowDesc(std::move(wstrWindowName), 1200, 700))
		, m_pInput(::std::make_shared<UserInput>(this->GetWindowDesc()))
    { } 

    template<class U>
    explicit EmptyCanvas(U&& desc)
        : IBaseWindow<EmptyCanvas, GamePolicy>(::std::forward<U>(desc))
    { }

public:

    const ::std::shared_ptr<UserInput>& GetInput() const
	{ return m_pInput; }

private:

    /**
    * @brief It's called on every update 
    */
    void HandleMessageImpl(const float fDelta, EAbWindowEventsFlags events)
    { 
        if constexpr (!bManualInputUpdate) {
            m_pInput->Update(fDelta);
        }
    }

private:

	::std::shared_ptr<UserInput> m_pInput;

};

} // !B33::App
#endif // !AB_EMPTYCANVAS_H

