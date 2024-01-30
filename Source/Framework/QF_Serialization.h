
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

namespace QwerkE {

    namespace Serialization
    {
        // #FEATURE F0001

        void DeserializeJsonObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void DeserializeJsonObject(const cJSON* objJson, T& obj)
        {
            DeserializeJsonObject(objJson, Mirror::InfoForClass<T>(), (void*)&obj);
        }

        template <class T>
        void DeserializeObjectByTypeKey(const cJSON* jsonObj, T& obj)
        {
            auto typeInfo = Mirror::InfoForType<T>();

            // #TODO Is the item guaranteed to be the root?
            DeserializeJsonObject<T>(GetItemFromRootByKey(jsonObj, typeInfo->stringName.c_str()), obj);
        }

        // #TODO Swap argument order to make deserialize and serialize a little more visually different to work with
        void SerializeJsonObject(cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);
        void SerializeJsonField(cJSON* objJson, const Mirror::Field& field, void* obj);

        template <class T>
        void SerializeObject(cJSON* objJson, const T& obj)
        {
            cJSON* arr = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
            SerializeJsonObject(arr, Mirror::InfoForClass<T>(), (void*)&obj);
            AddItemToObject(objJson, arr);
        }

    }

}
