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
#include "QF_EventManager.h"
#include "QF_Event.h"
#include "QF_AssetLoadedEvent.h"

namespace QwerkE {

    void* LoadAssetData(void* value);

    class QJob
    {
    public:
        virtual void Process() = 0;
    };

    class QLoadAsset : public QJob
    {
    public:
        QLoadAsset(const char* assetName)
        {
            m_AssetName = DeepCopyString(assetName);
        }

        void Process()
        {
            pthread_t threadID;
            pthread_create(&threadID, NULL, LoadAssetData, (void*)m_AssetName);
        };
    private:
        const char* m_AssetName;
    };

    class Jobs final // #TODO Consider for static namespace
    {
    public:
        static void ScheduleTask(QJob* job);

        static void ProcessTasks();

    private:
        Jobs() = default;

        static std::queue<QJob*> m_JobList;
        static void ProcessNextTask();
    };

}
