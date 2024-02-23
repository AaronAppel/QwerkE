
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Log.h"

namespace QwerkE {

    namespace Serialization {

        void DeserializeJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        template <class T>
        void DeserializeJsonFromFile(const char* fileName, T& objectReference) // #TODO Handle pointers as well, to avoid need to always dereference (unsafe)
        {
            if (!fileName)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* rootJsonObject = OpencJSONStream(fileName))
            {
                const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                if (!rootJsonObject->child)
                {
                    LOG_ERROR("{0} root JSON object has no children in JSON file {1} !", __FUNCTION__, fileName);
                }
                else if (strcmp(rootJsonObject->child->string, typeInfo->stringName.c_str()) != 0)
                {
                    LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, typeInfo->stringName.c_str());
                }
                else
                {
                    DeserializeJsonToObject(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
                    // DeserializeJsonToObject(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
                }

                ClosecJSONStream(rootJsonObject);
            }
            else
            {
                LOG_ERROR("{0} Could not load object type {1} from file {2} !", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), fileName);
            }
        }

        void SerializeObjectToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);
        // void SerializeObjectToJson(const void* obj, const Mirror::ClassInfo* objClassInfo, cJSON* objJson);

        template <class T>
        void SerializeObjectToFile(const T& objectReference, const char* filePath)
        {
            if (!filePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            cJSON* jsonRootObject = cJSON_CreateObject();
            cJSON* jsonRootArray = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
            AddItemToObject(jsonRootObject, jsonRootArray);

            SerializeObjectToJson((const void*)&objectReference, Mirror::InfoForType<T>(), jsonRootArray);

            PrintRootObjectToFile(filePath, jsonRootObject);
            cJSON_Delete(jsonRootObject);
        }

    }

}
