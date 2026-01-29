#include "B33Core.h"

#include "Synchronization/DeltaTime.hpp"

namespace B33::Core
{

using namespace ::std;
using namespace ::std::chrono;

// --------------------------------------------------------------------------------------------------------------------
DeltaTime::DeltaTime()
    : m_LastTimeStamp(ClockType::now())
{ }

// --------------------------------------------------------------------------------------------------------------------
void DeltaTime::SetReferenceFrame()
{ m_LastTimeStamp = ClockType::now(); }

// --------------------------------------------------------------------------------------------------------------------
float DeltaTime::FetchMs() 
{ 
    auto now = ClockType::now();
    float diff = duration_cast<DurationMs>(now - m_LastTimeStamp).count();
    m_LastTimeStamp = now;
    return diff; 
}

// --------------------------------------------------------------------------------------------------------------------
float DeltaTime::DeltaMs() 
{ 
    auto now = ClockType::now();
    float diff = duration_cast<DurationMs>(now - m_LastTimeStamp).count();
    return diff; 
}

} //!B33::Core
