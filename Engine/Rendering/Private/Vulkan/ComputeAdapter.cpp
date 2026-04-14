#include "B33Rendering.hpp"

#include "Vulkan/ComputeAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
const vector<const char *> &GraphicsComputeAdapter::GetExtensionsImpl() const
{
    return m_vExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void *GraphicsComputeAdapter::GetFeaturesImpl() const
{
    return NULL;
}

}; // namespace B33::Rendering
