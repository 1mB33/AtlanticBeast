#include "B33Rendering.hpp"

#include "Vulkan/ComputeAdapter.hpp"

namespace B33::Rendering
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
const vector<const char *> &ComputeAdapter::GetExtensionsImpl() const
{
    static const vector<const char *> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    return vpszDeviceExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void *ComputeAdapter::GetFeaturesImpl() const
{
    return NULL;
}

}; // namespace B33::Rendering
