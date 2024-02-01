
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Log.h" // #TODO Remove
#include "QF_Settings.h" // #TODO Remove

namespace QwerkE {

    namespace Serialization
    {
        // #FEATURE F0001

        // #TODO Deprecate
        void DeserializeJsonObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void DeserializeJsonObject(const cJSON* objJson, T& obj)
        {
            DeserializeJsonObject(objJson, Mirror::InfoForClass<T>(), (void*)&obj);
        }

        template <class T>
        void DeserializeObjectByTypeKey(const cJSON* jsonObj, T& obj)
        {
            // #TODO Is the item guaranteed to be the root?
            DeserializeJsonObject<T>(GetItemFromArrayByKey(jsonObj, Mirror::InfoForType<T>()->stringName.c_str()), obj);
        }

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void DeserializeObjectFromJsonFile(const cJSON* fileRootJson, T& obj)
        {
            if (!fileRootJson)
            {
                LOG_ERROR("{0} null argument(s) passed!", __FUNCTION__);
                return;
            }

            if (fileRootJson->type != cJSON_Object)
            {
                LOG_ERROR("{0} fileRootJson is not an object or root of JSON file!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
            if (strcmp(fileRootJson->child->string, typeInfo->stringName.c_str()) != 0)
            {
                // Verify that the file (only) contains a data instance of the object type
                LOG_ERROR("{0} fileRootJson 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, fileRootJson->child->string, typeInfo->stringName.c_str());
                return;
            }

            DeserializeJsonArrayToObject(fileRootJson->child, Mirror::InfoForClass<T>(), (void*)&obj);
        }

        // #TODO Swap argument order to make deserialize and serialize a little more visually different to work with
        void SerializeJsonObject(cJSON* objJson, const Mirror::ClassInfo* objClassInfo, const void* obj);

        template <class T>
        void SerializeObject(cJSON* objJson, const T& obj)
        {
            cJSON* arr = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
            SerializeJsonObject(arr, Mirror::InfoForClass<T>(), (const void*)&obj);
            AddItemToObject(objJson, arr);
        }

    }

}
