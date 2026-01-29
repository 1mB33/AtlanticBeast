#ifndef AB_MEMORY_H
#define AB_MEMORY_H

#include "WrapperHardware.hpp"
#include "WrapperAdapter.hpp"
#include "GPUBuffer.hpp"
#include "GPUStreamBuffer.hpp"

namespace B33::Rendering
{

struct UploadDescriptor
{
    enum EUploadType 
    {
        LocalBuffer,
        StreamBuffer,
    };

public:

    UploadDescriptor() = delete;
    
    template<typename U, typename L, class T>
    UploadDescriptor(U&& bufferInfo,
                     L&& write,
                     EUploadType type,
                     const T& buffer)
        : BufferInfo(::std::forward<U>(bufferInfo))
        , Write(::std::forward<L>(write))
        , Type(type)
    {
        Write.pBufferInfo = &BufferInfo;
        Buffer = buffer;
    }

    ~UploadDescriptor() 
    { }

public:

    UploadDescriptor(const UploadDescriptor& other) noexcept 
        : BufferInfo(other.BufferInfo)
        , Write(other.Write)
        , Type(other.Type)
    { 
        Write.pBufferInfo = &BufferInfo;
        Buffer = other.Buffer;
    }
    
    UploadDescriptor& operator=(const UploadDescriptor& other) noexcept
    {
        BufferInfo = other.BufferInfo;
        Write = other.Write;
        Type = other.Type;
        Write.pBufferInfo = &BufferInfo;
        Buffer = other.Buffer;
        
        return *this;
    }

    UploadDescriptor(UploadDescriptor&& other) noexcept 
        : BufferInfo(std::move(other.BufferInfo))
        , Write(std::move(other.Write))
        , Type(other.Type)
    { 
        Write.pBufferInfo = &BufferInfo;
        Buffer = std::move(other.Buffer);
    }

    UploadDescriptor& operator=(UploadDescriptor&& other) noexcept
    {
        BufferInfo = std::move(other.BufferInfo);
        Write = std::move(other.Write);
        Type = std::move(other.Type);
        Write.pBufferInfo = &BufferInfo;
        Buffer = other.Buffer;

        return *this;
    }

public:

    ::VkDescriptorBufferInfo BufferInfo;
    ::VkWriteDescriptorSet Write;
    EUploadType Type;
    ::std::shared_ptr<GPUBuffer> Buffer;

};


class Memory 
{
public:
    
    Memory() = default;

    Memory(::std::shared_ptr<const ::B33::Rendering::HardwareWrapper> pHardware,
           ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper> pAdapter);

    ~Memory() = default;

public:

    Memory(const Memory&) noexcept = default;
    Memory& operator=(const Memory&) noexcept = default;

    Memory(Memory&&) noexcept = default;
    Memory& operator=(Memory&&) noexcept = default;

public:

    BEAST_API ::std::shared_ptr<::B33::Rendering::GPUStreamBuffer> ReserveStagingBuffer(const ::size_t uSizeInBytes);

    BEAST_API ::std::shared_ptr<::B33::Rendering::GPUBuffer> ReserveGPUBuffer(const ::size_t uSizeInBytes);

    BEAST_API void UploadOnStreamBuffer(const void* pUpload, 
                                        const ::size_t uUploadSize,
                                        ::B33::Rendering::UploadDescriptor& onSet);

private:

    ::uint32_t FindMemoryType(::uint32_t typeFilter, ::VkMemoryPropertyFlags properties);

private:

    ::std::shared_ptr<const ::B33::Rendering::HardwareWrapper>  m_pHardware = nullptr;
    ::std::shared_ptr<const ::B33::Rendering::AdapterWrapper>   m_pAdapter  = nullptr;

};

} //!B33::Rendering
#endif //!AB_MEMORY_H
