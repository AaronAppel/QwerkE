#include "QF_Jobs.h"

#include "QF_EventManager.h"
#include "QF_JobQueuedEvent.h"
#include "QF_FileUtilities.h"

#include "QF_QwerkE_Defines.h"

namespace QwerkE {

    std::queue<QJob*> Jobs::m_JobList;

    void Jobs::ScheduleTask(QJob* job)
    {
        // TODO: Think of avoiding duplicate jobs
        m_JobList.push(job);
        Event* _event = new JobQueuedEvent();
        EventManager::QueueEvent(_event);
    }

    void Jobs::ProcessTasks()
    {
        for (size_t i = 0; i < m_JobList.size(); i++)
        {
            QJob* next = m_JobList.front();
            m_JobList.pop();

            next->Process();
        }
    }

    void Jobs::ProcessNextTask()
    {
        QJob* next = m_JobList.front();
        m_JobList.pop();

        next->Process();
    }

    void* LoadAssetData(void* value)
    {
        // TODO: Support all types of assets or files

        QImageFile fileData;
        fileData.s_FileName = (char*)value;

        if (FileExists(TexturesFolderPath(fileData.s_FileName.c_str())))
        {
            fileData.s_Data = (char*)FileSystem::LoadImageFileData(TexturesFolderPath(fileData.s_FileName.c_str()), &fileData.s_Width, &fileData.s_Height, (GLenum&)fileData.s_Channels, false);
        }

        if (fileData.s_Data != nullptr)
        {
            AssetLoadedEvent* _event = new AssetLoadedEvent(fileData);
            EventManager::QueueEvent(_event);
        }

        pthread_exit(NULL);

        return NULL;
    }

}
