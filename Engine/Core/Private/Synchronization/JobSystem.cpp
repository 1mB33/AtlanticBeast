#include "Synchronization/JobSystem.hpp"

namespace B33::Core
{

using namespace std;
using namespace B33;

void JobSystem::JobProcessorLoop( mutex              &mutex,
                                  condition_variable &condition,
                                  atomic_bool        &IsWorking,
                                  atomic_bool        &IsFree,
                                  Job                &currentJob )
{
    IsFree.store( true );

    while ( 1 )
    {
        unique_lock ul( mutex );
        condition.wait( ul,
                        [ & ]
                        {
                            return !IsFree.load() || !IsWorking.load();
                        } );

        if ( currentJob.Runnable != nullptr )
            currentJob.Runnable();

        if ( !IsWorking.load() )
            return;

        IsFree.store( true );
        ul.unlock();
        condition.notify_all();
    }
}

JobSystem::JobSystem()
  : m_Threads( 4 )
  , m_uHead( 0 )
{
    for ( auto &t : m_Threads )
    {
        B33_TRACE( L"JobSystem::JobSystem(): Starting one of job processors" );
        t.IsFree.store( true );
        t.IsWorking.store( true );
        t.CurrentJob = { nullptr };
        t.Thread     = thread( &JobSystem::JobProcessorLoop,
                               ref( t.Mutex ),
                               ref( t.Condition ),
                               ref( t.IsWorking ),
                               ref( t.IsFree ),
                               ref( t.CurrentJob ) );
    }
}

JobSystem::~JobSystem()
{
    for ( auto &t : m_Threads )
    {
        B33_TRACE( L"JobSystem::~JobSystem(): Stopping one of job processors" );
        t.IsWorking.store( false );
        t.Condition.notify_all();
        t.Thread.join();
    }
}

void JobSystem::BlockAndWait()
{
    for ( auto &t : m_Threads )
    {
        if ( !t.IsFree.load() )
        {
            unique_lock ul( t.Mutex );
            t.Condition.wait( ul,
                              [ & ]()
                              {
                                  return t.IsFree.load();
                              } );
        }
    }
}

void JobSystem::PushJobInternal( Job newJob )
{
    auto &headThread = m_Threads[ m_uHead ];

    if ( !headThread.IsFree.load() )
    {
        unique_lock ul( headThread.Mutex );
        headThread.Condition.wait( ul,
                                   [ & ]()
                                   {
                                       return headThread.IsFree.load();
                                   } );
    }

    headThread.CurrentJob = newJob;
    headThread.IsFree.store( false );
    headThread.Condition.notify_all();

    m_uHead = ( m_uHead + 1 ) % m_Threads.size();
}

}; // namespace B33::Core
