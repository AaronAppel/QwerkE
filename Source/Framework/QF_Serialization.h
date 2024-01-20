
#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

namespace QwerkE {

    namespace Serialization
    {
        void DeserializeJsonObject(const cJSON* objJson, const Reflection::ClassInfo* objClassInfo, void* obj);
        void DeserializeJsonString(const cJSON* jsonObj, const Reflection::Field& field, void* obj);
        void DeserializeJsonNumber(const cJSON* jsonObj, const Reflection::Field& field, void* obj);

        template <class T>
        void DeserializeJsonObject(const cJSON* objJson, T& obj)
        {
            DeserializeJsonObject(objJson, Reflection::GetClass<T>(), (void*)&obj);
        }

        template <class T>
        void DeserializeObjectByTypeKey(const cJSON* jsonObj, T& obj)
        {
            auto typeInfo = Reflection::GetTypeInfo<T>();

            // #TODO Is the item guaranteed to be the root?
            DeserializeJsonObject<T>(GetItemFromRootByKey(jsonObj, typeInfo->stringName.c_str()), obj);
        }

        // #TODO Swap argument order to make deserialize and serialize a little more visually different to work with
        void SerializeJsonObject(cJSON* objJson, const Reflection::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void SerializeObject(cJSON* objJson, const T& obj)
        {
            const Reflection::TypeInfo* typeInfo = Reflection::GetTypeInfo<T>();
            cJSON* arr = CreateArray(typeInfo->stringName.c_str());
            SerializeJsonObject(arr, Reflection::GetClass<T>(), (void*)&obj);
            AddItemToRoot(objJson, arr);
        }

    }

}