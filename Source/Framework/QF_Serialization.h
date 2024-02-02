
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Log.h" // #TODO Remove
#include "QF_Settings.h" // #TODO Remove

namespace QwerkE {

    // class cJSON;
    class Component;
    class GameObject;
    class Routine;

    // #FEATURE F0001
    // #TODO Consider splitting out to a new library. Maybe call it Serializer, or something cool.
    // Have it stand on it's own, but also be extensible to new types, libraries, and formats.
    // Probably needs to depend on Mirror, but keep them otherwise separate.
    namespace Serialization {

        GameObject* ConvertJSONToGameObject(cJSON* item);
        cJSON* ConvertGameObjectToJSON(void* obj);
        Component* AddComponentToGameObject(cJSON* item);
        Routine* AddRoutineToGameObject(cJSON* item);

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void DeserializeObjectFromJsonFile(const cJSON* fileRootJson, T& obj, bool verifyRoot = false)
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
            if (verifyRoot && strcmp(fileRootJson->child->string, typeInfo->stringName.c_str()) != 0)
            {
                // #TODO Verify that the file (only) contains a data instance of the object type
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
