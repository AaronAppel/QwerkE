
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Log.h" // #TODO Remove
#include "QF_Settings.h" // #TODO Remove

#include "QF_TransformRoutine.h"
#include "QF_RenderComponent.h"

#include "QF_LightComponent.h"

// #TODO Make a note of key/value naming. Variable names should be used when possible.
// This is safer when using string matching. For primitives and complex types, variable
// names should be available. For array elements, the type name will do fine.

namespace QwerkE {

    // class cJSON;
    class Component;
    class GameObject;
    class Material;
    class Model;
    class RenderComponent;
    class Routine;
    class ShaderProgram;

    // #FEATURE F0001
    // #TODO Consider splitting out to a new library. Maybe call it Serializer, or something cool.
    // Have it stand on it's own, but also be extensible to new types, libraries, and formats.
    // Probably needs to depend on Mirror, but keep them otherwise separate.
    namespace Serialization {

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);
        void DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj);
        void DeserializeJsonNumber(const cJSON* jsonObj, const MirrorTypes type, const unsigned int size, void* obj);
        void DeserializeJsonString(const cJSON* jsonObj, const MirrorTypes type, void* obj);

        // template <class T>
        // void DeserializeClassFromFile(const char* fileName, T& objectReference)
        // {
        //     const Mirror::ClassInfo* classInfo = Mirror::InfoForClass<T>();
        //
        //     for (size_t i = 0; i < classInfo->fields.size(); i++)
        //     {
        //         classInfo->fields.at(i);
        //     }
        // }

        template <class T>
        void DeserializeJsonFromFile(const char* fileName, T& objectReference)
        {
            if (!fileName)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            // #TODO Handle different file extensions?
            cJSON* rootJsonObject = OpencJSONStream(fileName);
            if (rootJsonObject)
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

        // #TODO Swap argument order to make deserialize and serialize a little more visually different to work with
        void SerializeJsonObject(cJSON* objJson, const Mirror::ClassInfo* objClassInfo, const void* obj);

        template <class T>
        void SerializeObject(cJSON* objJson, const T& obj)
        {
            cJSON* arr = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
            SerializeJsonObject(arr, Mirror::InfoForClass<T>(), (const void*)&obj);
            AddItemToObject(objJson, arr);
        }

        template <class T>
        void SerializeObjectToFile(const char* filePath, const T& objectReference)
        {
            if (!filePath)
            {
                LOG_ERROR("{0} Null file path given!", __FUNCTION__);
                return;
            }

            cJSON* jsonRootObject = cJSON_CreateObject();
            if (jsonRootObject)
            {
                cJSON* rootArray = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
                // Serialization::SerializeObject(jsonRootObject, objectReference);
                SerializeJsonObject(rootArray, Mirror::InfoForClass<T>(), (const void*)&objectReference);
                AddItemToObject(jsonRootObject, rootArray);
                PrintRootObjectToFile(filePath, jsonRootObject);
                cJSON_Delete(jsonRootObject);
            }
            else
            {
                LOG_ERROR("{0} Unable to serialize object for path {1}!", __FUNCTION__, filePath);
            }
        }

        cJSON* ConvertGameObjectToJSON(void* obj);

    }

}
