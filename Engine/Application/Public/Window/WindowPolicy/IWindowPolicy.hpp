#ifndef AB_IWINDOW_POLICY_H
#define AB_IWINDOW_POLICY_H

#include "Window/WindowDesc.hpp"

namespace B33::App
{

template<class Policy>
class IWindowPolicy
{
public:

	IWindowPolicy() = default;

	~IWindowPolicy() = default;

public:

	IWindowPolicy(const IWindowPolicy&) = default;
	IWindowPolicy& operator=(const IWindowPolicy&) = default;

	IWindowPolicy(IWindowPolicy&&) noexcept = default;
	IWindowPolicy& operator=(IWindowPolicy&&) noexcept = default;

public:

	inline uint32_t WindowPolicyCreate(WindowDesc* pWd)
	{ return static_cast<Policy*>(this)->CreateImpl(pWd); }

	inline void WindowPolicyShow(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->ShowImpl(pWd); }

	inline void WindowPolicyHide(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->HideImpl(pWd); }

	inline void WindowPolicyDestroy(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->DestroyImpl(pWd); }

	inline void WindowPolicyUpdate(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->UpdateImpl(pWd); }

};

} // !B33::App
#endif // !AB_IWINDOW_POLICY_H
