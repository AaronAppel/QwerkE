
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Log.h"

// #TODO Make a note of key/value naming. Variable names should be used when possible.
// This is safer when using string matching. For primitives and complex types, variable
// names should be available. For array elements, the type name will do fine.

namespace QwerkE {

    // #FEATURE F0001
    // #TODO Consider splitting out to a new library. Maybe call it Serializer, or something cool.
    // Have it stand on it's own, but also be extensible to new types, libraries, and formats.
    // Probably needs to depend on Mirror, but keep them otherwise separate.

     // #TODO Mirror support for vectors, maps, and other collections

    namespace Serialization {

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);
        void DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj);
        void DeserializeJsonNumber(const cJSON* jsonObj, const MirrorTypes type, const unsigned int size, void* obj);
        void DeserializeJsonString(const cJSON* jsonObj, const MirrorTypes type, void* obj);

        template <class T>
        void DeserializeJsonFromFile(const char* fileName, T& objectReference)
        {
            if (!fileName)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* rootJsonObject = OpencJSONStream(fileName)) // #TODO Handle different file extensions?
            {
                const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                if (!rootJsonObject->child)
                {
                    LOG_ERROR("{0} root JSON object has no children in JSON file {1} !", __FUNCTION__, fileName);
                }
                else if (false && strcmp(rootJsonObject->child->string, typeInfo->stringName.c_str()) != 0)
                {
                    // #TODO Verify that the file (only) contains a data instance of the object type
                    LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, typeInfo->stringName.c_str());
                }
                else
                {
                    DeserializeJsonArrayToObject(rootJsonObject->child, Mirror::InfoForClass<T>(), (void*)&objectReference);
                }

                ClosecJSONStream(rootJsonObject);
            }
            else
            {
                LOG_ERROR("{0} Could not load object type {1} from file {2} !", __FUNCTION__, Mirror::InfoForType<T>()->stringName.c_str(), fileName);
            }
        }

        void SerializeJsonObject(const void* obj, const Mirror::ClassInfo* objClassInfo, cJSON* objJson);

        template <class T>
        void SerializeObject(const T& obj, cJSON* objJson)
        {
            cJSON* arr = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
            SerializeJsonObject(arr, Mirror::InfoForClass<T>(), (const void*)&obj);
            AddItemToObject(objJson, arr);
        }

        template <class T>
        void SerializeObjectToFile(const T& objectReference, const char* filePath)
        {
            if (!filePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            if (cJSON* jsonRootObject = cJSON_CreateObject())
            {
                cJSON* rootArray = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
                SerializeJsonObject((const void*)&objectReference, Mirror::InfoForClass<T>(), rootArray);
                AddItemToObject(jsonRootObject, rootArray);
                PrintRootObjectToFile(filePath, jsonRootObject);
                cJSON_Delete(jsonRootObject);
            }
            else
            {
                LOG_ERROR("{0} Unable to serialize object for path {1}!", __FUNCTION__, filePath);
            }
        }

    }

}
