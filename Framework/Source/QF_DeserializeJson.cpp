#include "QF_Serialization.h"


namespace QwerkE {

    namespace Serialization {

        void NewDeserializeToJson(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (true)
            {

            }

        }

    }

}
