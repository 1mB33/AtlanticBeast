#include "Input/InputEvents.h"
#if defined( __APPLE__ )

#    include "B33Core.h"

#    include "AppStatus.hpp"
#    include "Window/BaseWindowDetails.h"
#    include "Window/WindowPolicy/Apple/BasicApplePolicy.hpp"

namespace B33::App
{

using namespace ::B33::Core;
using namespace ::B33::Core::Debug;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicAppleWindowPolicy::CreateImpl( WindowDesc *pWd )
{
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::ShowImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::HideImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::DestroyImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicAppleWindowPolicy::UpdateImpl( WindowDesc *pWd )
{
    B33_ASSERT( pWd );
    B33_ASSERT( pWd->bIsAlive );
}


} // namespace B33::App
#endif // !__linux__

