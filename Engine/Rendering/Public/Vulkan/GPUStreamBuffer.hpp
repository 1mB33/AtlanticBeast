#ifndef AB_GPU_STREAM_BUFFER_H
#define AB_GPU_STREAM_BUFFER_H

#include "B33Rendering.hpp"
#include "Vulkan/GPUBuffer.hpp"

namespace B33::Rendering 
{

class GPUStreamBuffer : public GPUBuffer 
{
public:

    GPUStreamBuffer() 
        : GPUBuffer() 
        , m_pData(nullptr)
    { }

    GPUStreamBuffer(::std::shared_ptr<const AdapterWrapper> da,
                    VkDeviceMemory deviceMemory,
                    VkBuffer buffer,
                    void* pData,
                    size_t sizeInBytes) 
        : GPUBuffer(da, deviceMemory, buffer, sizeInBytes) 
    { }

    ~GPUStreamBuffer() 
    {
        if (m_pDeviceAdapter == nullptr) {
            return;
        }
        if (m_pData != nullptr) {
            vkUnmapMemory(m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory);
            m_pData = nullptr;
        }

        GPUBuffer::~GPUBuffer();
    }

public:

    GPUStreamBuffer(const GPUStreamBuffer& other) = delete;
    GPUStreamBuffer& operator=(const GPUStreamBuffer& other) noexcept = delete;

    GPUStreamBuffer(GPUStreamBuffer&& other) noexcept
        : GPUBuffer(std::move(other))
        , m_pData(other.m_pData)
    { }

    GPUStreamBuffer& operator=(GPUStreamBuffer&& other) noexcept
    {
        this->GPUBuffer::operator=(std::move(other));
        m_pData = other.m_pData;

        other.m_pData = nullptr;
        return *this;
    }

public:

    void* GetDataPointer() const
    { return m_pData; }

    void** GetPtrToDataPointer()
    { return &m_pData; }

public:

    void Reset()
    {
        vkUnmapMemory(this->m_pDeviceAdapter->GetAdapterHandle(), this->GetMemoryHandle());
        m_pData = nullptr;
    }

private:

    void* m_pData = nullptr;

};

} // !B33::Rendering
#endif // !AB_GPU_STREAM_BUFFER_H
