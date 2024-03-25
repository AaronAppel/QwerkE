#include "QF_Jobs.h"

#include <queue>

#ifdef _QPTHREADS
#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC // #TODO Remove have struct pthread order dependency
#endif
#include "Libraries/pThreads/pthread.h"
#endif

#include "QC_Time.h"

#include "QF_Events.h"
#include "QF_FileUtilities.h"
#include "QF_JobQueuedEvent.h"
#include "QF_Log.h"
#include "QF_Settings.h"

#include "QF_PathDefines.h"

namespace QwerkE {

    namespace Jobs {

        static std::queue<QJob*> s_JobList;
        static const u8 s_MaxQueuedJobs = 50;
        static u8 s_CurrentActiveJobs = 0;
        static float s_secondsBeforeNextTask = 0.f;

        void ScheduleJob(QJob* job)
        {
            if (s_JobList.size() < s_MaxQueuedJobs)
            {
                s_JobList.push(job); // #TODO Look at avoiding duplicate jobs
            }
            else if (const QLoadAsset* assetLoadedJob = static_cast<QLoadAsset*>(job))
            {
                LOG_WARN("{0} Job list full. Discarding job {1}", __FUNCTION__, assetLoadedJob->AssetName());
            }
            else
            {
                LOG_WARN("{0} Job list full", __FUNCTION__);
            }
        }

        void ProcessTasks(const float minimumDelayBetweenTasksSec)
        {
            s_secondsBeforeNextTask -= Time::PreviousFrameDuration();
            if (s_secondsBeforeNextTask <= 0.f)
            {
                for (size_t i = 0; i < s_JobList.size(); i++)
                {
                    const u8 maxAdditionalThreadCount = Settings::GetEngineSettings().maxJobsAdditionalThreadCount;
                    const bool singleThreaded = maxAdditionalThreadCount < 1;

                    if (singleThreaded || s_CurrentActiveJobs < maxAdditionalThreadCount)
                    {
                        QJob* next = s_JobList.front();
                        next->Process(!singleThreaded);
                        s_JobList.pop();
                        delete next;
                        ++s_CurrentActiveJobs;
                        LOG_TRACE("{0} Job started. Active count {1}", __FUNCTION__, s_CurrentActiveJobs);
                        break;
                    }
                }
                s_secondsBeforeNextTask = minimumDelayBetweenTasksSec;
            }
        }

        void ProcessNextTask()
        {
            QJob* next = s_JobList.front();
            s_JobList.pop();
            next->Process();
            delete next;
        }

        void OnJobFinished()
        {
            --s_CurrentActiveJobs;
        }

        void* LoadAssetDataSync(void* value)
        {
            // TODO: Support all types of assets or files

            std::shared_ptr<QImageFile> fileData = std::make_shared<QImageFile>();
            fileData->s_FileName = (char*)value;
            LOG_TRACE("{0} File name: {1}", __FUNCTION__, fileData->s_FileName.c_str());

            if (FileExists(fileData->s_FileName.c_str()))
            {
                fileData->s_Data = (char*)Files::LoadImageFileData(fileData->s_FileName.c_str(), &fileData->s_Width, &fileData->s_Height, (GLenum&)fileData->s_Channels, false);
                LOG_TRACE("{0} File exists name: {1}", __FUNCTION__, fileData->s_FileName.c_str());
            }

            if (fileData->s_Data != nullptr)
            {
                AssetLoadedEvent* _event = new AssetLoadedEvent(fileData);
                Events::QueueEvent(_event);
                LOG_TRACE("{0} Event queued", __FUNCTION__);
            }

            if (value)
            {
                delete value;
            }

            Jobs::OnJobFinished();

            return nullptr;
        }

        void* LoadAssetDataAsync(void* value)
        {
            void* result = LoadAssetDataSync(value);

#ifdef _QPTHREADS
            pthread_exit(nullptr);
#endif


            return result;
        }

    }

}
