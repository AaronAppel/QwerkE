
#ifdef _QCJSON
#include "Libraries/cJSON/QC_cJSON.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Log.h"

namespace QwerkE {

    class Scene;

    namespace Serialization {

        void DeserializeJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        template <class T>
        void DeserializeObjectFromFile(const char* absoluteFilePath, T& objectReference) // #TODO Handle pointers as well, to avoid need to always dereference (unsafe)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* rootJsonObject = OpencJSONStream(absoluteFilePath))
            {
                const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                if (!rootJsonObject->child)
                {
                    LOG_ERROR("{0} root JSON object has no children in JSON file {1}!", __FUNCTION__, absoluteFilePath);
                }
                else if (strcmp(rootJsonObject->child->string, typeInfo->stringName.c_str()) != 0)
                {
                    LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, typeInfo->stringName.c_str());
                }
                else
                {
                    DeserializeJsonToObject(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
                }

                ClosecJSONStream(rootJsonObject);
            }
            else
            {
                LOG_ERROR("{0} Could not load object type {1} from file {2}!", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), absoluteFilePath);
            }
        }

        void SerializeObjectToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        template <class T>
        void SerializeObjectToFile(const T& objectReference, const char* absoluteFilePath)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

            cJSON* jsonRootObject = cJSON_CreateObject();
            cJSON* jsonRootArray = CreateArray(typeInfo->stringName.c_str());
            AddItemToObject(jsonRootObject, jsonRootArray);

            SerializeObjectToJson((const void*)&objectReference, typeInfo, jsonRootArray);

            PrintRootObjectToFile(absoluteFilePath, jsonRootObject);
            cJSON_Delete(jsonRootObject);
        }

        void NewDeserializeToJson(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        template <class T>
        void NewDeserializeObjectToFile(const char* absoluteFilePath, const T& objectReference)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* rootJsonObject = OpencJSONStream(absoluteFilePath))
            {
                const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                if (!rootJsonObject->child)
                {
                    LOG_ERROR("{0} root JSON object has no children in JSON file {1}!", __FUNCTION__, absoluteFilePath);
                }
                else if (strcmp(rootJsonObject->child->string, typeInfo->stringName.c_str()) != 0)
                {
                    LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, typeInfo->stringName.c_str());
                }
                else
                {
                    NewDeserializeToJson(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
                }

                ClosecJSONStream(rootJsonObject);
            }
            else
            {
                LOG_ERROR("{0} Could not load object type {1} from file {2}!", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), absoluteFilePath);
            }
        }

        void SerializeToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        template <class T>
        void NewSerializeObjectToFile(const T& objectReference, const char* absoluteFilePath)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

            cJSON* jsonRootObject = cJSON_CreateObject();

            SerializeToJson((const void*)&objectReference, typeInfo, jsonRootObject);

            PrintRootObjectToFile(absoluteFilePath, jsonRootObject);
            cJSON_Delete(jsonRootObject);
        }

    }

}
