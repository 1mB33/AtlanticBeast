#include "B33Rendering.hpp"

#include "Vulkan/ComputeAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
const vector<const char *> &ComputeAdapter::GetExtensionsImpl() const
{
    return m_vExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void *ComputeAdapter::GetFeaturesImpl() const
{
    return NULL;
}

}; // namespace B33::Rendering
