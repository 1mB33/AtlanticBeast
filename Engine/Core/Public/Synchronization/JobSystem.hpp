#if !defined( AB_JOB_SYSTEM_HPP )
#    define AB_JOB_SYSTEM_HPP

#    include "B33Core.h"

namespace B33::Core
{

class JobSystem
{
  public:
    BEAST_API JobSystem();

    BEAST_API ~JobSystem();

  public:
    template <typename FUNCTION>
    void PushJob( FUNCTION fn )
    {
        B33_TRACE( L"JobSystem::PushJob() Pushing new job" );
        Job newJob      = {};
        newJob.Runnable = fn;

        PushJobInternal( ::std::move( newJob ) );
    }

    void BlockAndWait();

  private:
    struct Job
    {
        ::std::function<void()> Runnable;
    };

    struct JobProcessor
    {
        ::std::thread             Thread;
        ::std::mutex              Mutex;
        ::std::atomic_bool        IsWorking;
        ::std::atomic_bool        IsFree;
        ::std::condition_variable Condition;
        Job                       CurrentJob;
    };

  private:
    void PushJobInternal( Job newJob );

    void PopJobInternal();

    void StealJobInternal();

    static void JobProcessorLoop( ::std::mutex              &mutex,
                                  ::std::condition_variable &condition,
                                  ::std::atomic_bool        &IsWorking,
                                  ::std::atomic_bool        &IsFree,
                                  Job                       &currentJob );

  private:
    ::std::vector<JobProcessor> m_Threads = {};
    ::size_t                    m_uHead   = -1;
};

} // namespace B33::Core

#endif // !AB_JOB_SYSTEM_HPP
