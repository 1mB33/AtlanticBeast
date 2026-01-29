#ifndef AB_MEMORY_UPLOAD_TRACKER_H 
#define AB_MEMORY_UPLOAD_TRACKER_H

namespace B33::Rendering
{

enum EReupload
{
    NoAction = 1,
    RequestStaging = NoAction << 1,
    RequestGpuUpload = RequestStaging << 1,
};

class MemoryUploadTracker
{
public:

    MemoryUploadTracker()
        : m_Reupload(RequestStaging)
    { }

    ~MemoryUploadTracker() = default;

public:

    MemoryUploadTracker(const MemoryUploadTracker&) noexcept = default;
    MemoryUploadTracker& operator=(const MemoryUploadTracker&) noexcept = default;

    MemoryUploadTracker(MemoryUploadTracker&&) noexcept = default;
    MemoryUploadTracker& operator=(MemoryUploadTracker&&) noexcept = default;

public:

    /**
     * @brief Returns status that the upload is on, shifts the value to next stage
     *
     * @return EReupload enumerator that descirbes the stage
     */
    EReupload ReuploadStatus()
    {
        switch (m_Reupload) {
            case EReupload::NoAction:
                return EReupload::NoAction;
            case EReupload::RequestStaging:
                m_Reupload = EReupload::RequestGpuUpload;
                return EReupload::RequestStaging;
            case EReupload::RequestGpuUpload:
                m_Reupload = EReupload::NoAction;
                return EReupload::RequestGpuUpload;
            default:
                return EReupload::NoAction;
        }
    }

public:

    void ForceUpload()
    { 
        if (m_Reupload & EReupload::RequestStaging)
            return;

        m_Reupload = EReupload::RequestStaging; 
    }

private:

    EReupload m_Reupload = NoAction;

};

} //!B33::Rendering
#endif //!AB_MEMORY_UPLOAD_TRACKER_H
