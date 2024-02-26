#pragma once

/*
	An interface for multi threaded tasks
	https://computing.llnl.gov/tutorials/pthreads/
*/

#include <queue>

// #TODO Remove have struct pthread order dependency
#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC
#endif
#include "Libraries/pThreads/pthread.h"

#include "QC_StringHelpers.h"
#include "QF_Event.h"
#include "QF_Events.h"
#include "QF_AssetLoadedEvent.h"

namespace QwerkE {

    class QJob;
    namespace Jobs {

        void ScheduleJob(QJob* job);

        void ProcessTasks(float minimumDelayBetweenTasksSec = 0.f);

        void ProcessNextTask();

        void OnJobFinished();

        void* LoadAssetDataAsync(void* value);
        void* LoadAssetDataSync(void* value);

    }

    class QJob
    {
    public:
        virtual void Process(bool aSync = true) = 0;
    };

    class QLoadAsset : public QJob
    {
    public:
        QLoadAsset(const char* assetName) { m_AssetName = DeepCopyString(assetName); }
        ~QLoadAsset() { if (m_AssetName) delete m_AssetName; }

        void Process(bool aSync = true)
        {
            if (aSync)
            {
                pthread_t threadID; // #TODO Track thread, or delete? What happens when this scope ends? Does the thread go out of scope?
                pthread_create(&threadID, NULL, Jobs::LoadAssetDataAsync, (void*)m_AssetName);
                // #TODO pthread_join or kill at some point on early exit
            }
            else
            {
                Jobs::LoadAssetDataSync((void*)m_AssetName);
            }
        };

        const char* const AssetName() const { return m_AssetName; }

    private:
        const char* m_AssetName;
    };

}
