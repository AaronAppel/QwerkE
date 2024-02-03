
#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Log.h" // #TODO Remove
#include "QF_Settings.h" // #TODO Remove

// #TODO Make a note of key/value naming. Variable names should be used when possible.
// This is safer when using string matching. For primitives and complex types, variable
// names should be available. For array elements, the type name will do fine.

namespace QwerkE {

    // class cJSON;
    class Component;
    class GameObject;
    class Material;
    class RenderComponent;
    class Routine;
    class ShaderProgram;

    // #FEATURE F0001
    // #TODO Consider splitting out to a new library. Maybe call it Serializer, or something cool.
    // Have it stand on it's own, but also be extensible to new types, libraries, and formats.
    // Probably needs to depend on Mirror, but keep them otherwise separate.
    namespace Serialization {

        void DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj);
        void DeserializeJsonNumber(const cJSON* jsonObj, const MirrorTypes type, const unsigned int size, void* obj);
        void DeserializeJsonString(const cJSON* jsonObj, const MirrorTypes type, void* obj);

        void DeserializeJsonArrayToObject(const cJSON* objJson, const Mirror::ClassInfo* objClassInfo, void* obj);

        template <class T>
        void DeserializeObject(const cJSON* fileRootJson, T& objectReference)
        {
            // Take a JSON object that may exist within a structure,
            // write it's contents to the given objectReference.

            if (!fileRootJson || fileRootJson->type == cJSON_NULL)
            {
                LOG_ERROR("{0} null JSON object passed!", __FUNCTION__);
                return;
            }

            switch (fileRootJson->type)
            {
            case cJSON_False:
            case cJSON_True:
            case cJSON_Number:

                break;

            case cJSON_String:
                {
                    const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                    DeserializeJsonString(fileRootJson, typeInfo->enumType, (char*)objectReference);
                }
                break;

                case cJSON_Object:
                    break;

                default:
                    break;
            }

            if (!fileRootJson->child)
            {
                // Check if it is a primitive type
            }

            if (fileRootJson->type = cJSON_Object)
            {
                fileRootJson = fileRootJson->child; // #TODO Improve logic
            }

            DeserializeJsonArrayToObject(fileRootJson, Mirror::InfoForClass<T>(), (void*)&objectReference);
        }

        template <class T>
        void DeserializeObjectFromFile(const char* fileName, T& objectReference)
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

        GameObject* ConvertJSONToGameObject(cJSON* item);
        cJSON* ConvertGameObjectToJSON(void* obj);
        Component* AddComponentToGameObject(cJSON* item);
        Routine* AddRoutineToGameObject(cJSON* item);

    }

}
