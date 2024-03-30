#pragma once

#include <memory>

#include "QF_Event.h"
#include "QF_Files.h"

namespace QwerkE {

    class AssetLoadedEvent : public Event
    {
    public:
        AssetLoadedEvent(std::shared_ptr<QImageFile> asset); // #TODO Reference a QFile, or rename class to be more specific to image files
        void Process();

    private:
        std::shared_ptr<QImageFile> m_Asset;
    };

}
