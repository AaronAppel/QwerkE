
#ifdef _QCJSON
#include "Libraries/cJSON/cJSON.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Files.h"
#include "QF_Log.h"

struct cJSON;

namespace QwerkE {

    class Scene;

    namespace Serialization {

        // #TODO Move somewhere better
        cJSON* ParseJsonFile(const char* absoluteFilePath);
        void PrintJsonToFile(cJSON* jsonRootObject, const char* absoluteFilePath);
        cJSON* TempCreateArray(const char* key);

        void DeserializeJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        template <class T>
        void DeserializeObjectFromFile(const char* absoluteFilePath, T& objectReference) // #TODO Handle pointers as well, to avoid need to always dereference (unsafe)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* rootJsonObject = ParseJsonFile(absoluteFilePath))
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
                cJSON_Delete(rootJsonObject);
            }
            else
            {
                LOG_ERROR("{0} Could not load object type {1} from file {2}!", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), absoluteFilePath);
            }
            return; // #TODO Remove code after testing

            // if (cJSON* rootJsonObject = OpencJSONStream(absoluteFilePath))
            // {
            //     const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
            //     if (!rootJsonObject->child)
            //     {
            //         LOG_ERROR("{0} root JSON object has no children in JSON file {1}!", __FUNCTION__, absoluteFilePath);
            //     }
            //     else if (strcmp(rootJsonObject->child->string, typeInfo->stringName.c_str()) != 0)
            //     {
            //         LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, typeInfo->stringName.c_str());
            //     }
            //     else
            //     {
            //         DeserializeJsonToObject(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
            //     }
            //     cJSON_Delete(rootJsonObject);
            // }
            // else
            // {
            //     LOG_ERROR("{0} Could not load object type {1} from file {2}!", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), absoluteFilePath);
            // }
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
            // cJSON* jsonRootArray = TempCreateArray(typeInfo->stringName.c_str());

            // #TODO Move to new function
            cJSON* jsonRootArray = cJSON_CreateArray();
            jsonRootArray->child = cJSON_CreateObject();
            jsonRootArray->string = _strdup(typeInfo->stringName.c_str());

            cJSON_AddItemToArray(jsonRootObject, jsonRootArray);

            SerializeObjectToJson((const void*)&objectReference, typeInfo, jsonRootArray);

            PrintJsonToFile(jsonRootObject, absoluteFilePath);
            cJSON_Delete(jsonRootObject);
        }

        void SerializeToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

        template <class T>
        void NewSerializeToFile(const T& objectReference, const char* absoluteFilePath)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            cJSON* jsonRootObject = cJSON_CreateObject();

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
            SerializeToJson((const void*)&objectReference, typeInfo, jsonRootObject);

            PrintJsonToFile(jsonRootObject, absoluteFilePath);
            cJSON_Delete(jsonRootObject);
        }

        void DeserializeFromJson(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj);

        template <class T>
        void NewDeserializeFromFile(const char* absoluteFilePath, T& objectReference)
        {
            if (!absoluteFilePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* rootJsonObject = ParseJsonFile(absoluteFilePath))
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
                    DeserializeFromJson(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
                }
                cJSON_Delete(rootJsonObject);
            }
            else
            {
                LOG_ERROR("{0} Could not load object type {1} from file {2}!", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), absoluteFilePath);
            }
        }

    }

}
