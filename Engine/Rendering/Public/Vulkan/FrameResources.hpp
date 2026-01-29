#ifndef AB_FRAME_RESOURCES_H
#define AB_FRAME_RESOURCES_H

namespace B33::Rendering
{

struct Frame
{
    constexpr static size_t MAX_FRAMES_IN_FLIGHT = 2;

	::VkFence			InFlightFence;
	::VkSemaphore		ImageAvailable;
	::VkSemaphore		RenderFinished;
	::VkCommandBuffer   CommandBuffer;
};

} // !B33::Rendering
#endif // !AB_FRAME_RESOURCES_H
