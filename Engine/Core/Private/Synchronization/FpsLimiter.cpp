#include "B33Core.h"

#include "Synchronization/FpsLimiter.hpp"

namespace B33::Core
{

using namespace ::std;
using namespace ::std::chrono;

// --------------------------------------------------------------------------------------------------------------------
FpsLimiter::FpsLimiter(const float fTargetMs)
    : m_fTarget(fTargetMs)
    , m_fBalance(0.f)
{ }

// --------------------------------------------------------------------------------------------------------------------
float FpsLimiter::Block(const float fDeltaMs, const float fFetchMs)
{
    using DurationMs = duration<float, milli>;
    
    m_fBalance += m_fTarget - fFetchMs;
    const float fTotalWait = m_fTarget - fDeltaMs + m_fBalance;
    if (fTotalWait <= 0.f) {
        m_fBalance *= 0.5f;
        return 0.f;
	}

    this_thread::sleep_for(DurationMs(fTotalWait));

    return fTotalWait;
}

} //!B33::Core
