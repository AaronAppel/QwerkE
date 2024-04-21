#include "QF_Serialization.h"

#include <unordered_map>
#include <string>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#include "QC_Guid.h"

// Game scripts
#include "QF_Scripting.h"
#include "QF_Scriptable.h"
#include "QF_ScriptPatrol.h"

// Editor types
#include "../Editor/Source/QE_EditorWindow.h"
#include "../Editor/Source/QE_EditorWindowAssets.h"
#include "../Editor/Source/QE_EditorWindowDefaultDebug.h"
#include "../Editor/Source/QE_EditorWindowDockingContext.h"
#include "../Editor/Source/QE_EditorWindowEntityInspector.h"
#include "../Editor/Source/QE_EditorWindowFolderViewer.h"
#include "../Editor/Source/QE_EditorWindowImGuiDemo.h"
#include "../Editor/Source/QE_EditorWindowMaterialEditor.h"
#include "../Editor/Source/QE_EditorWindowMenuBar.h"
#include "../Editor/Source/QE_EditorWindowNodeEditor.h"
#include "../Editor/Source/QE_EditorWindowSceneControls.h"
#include "../Editor/Source/QE_EditorWindowSceneGraph.h"
#include "../Editor/Source/QE_EditorWindowSceneView.h"
#include "../Editor/Source/QE_EditorWindowSettings.h"
#include "../Editor/Source/QE_EditorWindowShaderEditor.h"
#include "../Editor/Source/QE_EditorWindowStylePicker.h"

namespace QwerkE {

    namespace Serialization {

        void local_SerializeClass(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);
        void local_SerializeCollection(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);
        void local_SerializePrimitive(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);

        template <typename T>
        void SerializeComponent(EntityHandle& handle, cJSON* entityComponentsJsonArray)
        {
            if (!handle.HasComponent<T>())
                return;

            T& component = handle.GetComponent<T>();
            auto componentTypeInfo = Mirror::InfoForType<T>();
            cJSON* newJsonObjectArray = CreateArray(componentTypeInfo->stringName.c_str());
            SerializeObjectToJson(&component, componentTypeInfo, newJsonObjectArray);
            AddItemToArray(entityComponentsJsonArray, newJsonObjectArray);
        }

        void SerializeToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->isPointer)
            {
                LOG_ERROR("{0} Pointer types are not currently supported!", __FUNCTION__);
                return;
            }

            if (objTypeInfo->isPrimitive())
            {
                const std::string name = objTypeInfo->stringName;
                local_SerializePrimitive(obj, objTypeInfo, objJson, name);
            }
            else if (objTypeInfo->newIsClass())
            {
                local_SerializeClass(obj, objTypeInfo, objJson);
            }
            else if (objTypeInfo->newIsCollection())
            {
                local_SerializeCollection(obj, objTypeInfo, objJson);
            }
            else
            {
                LOG_ERROR("{0} Error deducing type!", __FUNCTION__);
            }
        }

        void local_SerializeClass(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (!objTypeInfo->isClass)
            {
                LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                return;
            }

            cJSON* classJsonContainer = cJSON_CreateObject();
            classJsonContainer->string = _strdup(objTypeInfo->stringName.c_str());
            AddItemToObject(objJson, classJsonContainer);

            if (objTypeInfo->isSubClass())
            {
                if (objTypeInfo->superTypeInfo)
                {
                    SerializeToJson(obj, objTypeInfo->superTypeInfo, classJsonContainer);
                }
                else
                {
                    LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                    return;
                }
            }

            if (objTypeInfo->fields.empty())
            {
                LOG_WARN("{0} Class has no serialized members", __FUNCTION__);
            }

            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];
                void* fieldAddress = (char*)obj + field.offset;

                if (field.typeInfo->isPrimitive()) // #TODO Review handling primitive here (dependent on field name)
                {
                    const std::string name = name;
                    local_SerializePrimitive(fieldAddress, field.typeInfo, classJsonContainer, name);
                }
                else
                {
                    SerializeToJson(fieldAddress, field.typeInfo, classJsonContainer);
                }
            }
        }

        void local_SerializeCollection(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            if (false) // (!objTypeInfo->newIsCollection()) // || !objTypeInfo->collectionTypeInfo)
            {
                LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
                return;
            }

            cJSON* collectionJsonContainer = cJSON_CreateObject();
            collectionJsonContainer->string = _strdup(objTypeInfo->stringName.c_str());
            AddItemToObject(objJson, collectionJsonContainer);

            switch (objTypeInfo->enumType)
            {
                // imgui types
            case MirrorTypes::m_imvec4_array:
                {
                    if (objTypeInfo->isArray())
                        break;

                    const ImVec4* imVec4Array = (ImVec4*)obj;

                    for (size_t i = 0; i < ImGuiCol_COUNT; i++) // #TODO Get array size
                    {
                        SerializeObjectToJson(&imVec4Array[i], objTypeInfo, collectionJsonContainer);
                    }
                }
                break;
            // imgui types

            case MirrorTypes::m_pair_guid_string:
                {
                    using PairGuidString = std::pair<GUID, std::string>;
                    const PairGuidString* guidStringPair = (PairGuidString*)((char*)obj);

                    cJSON* pairJsonContainer = cJSON_CreateString(guidStringPair->second.c_str());
                    pairJsonContainer->string = _strdup(std::to_string(guidStringPair->first).c_str());
                    AddItemToObject(collectionJsonContainer, pairJsonContainer);
                }
                break;

            case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
                {
                    const std::unordered_map<eScriptTypes, Scriptable*>* scriptsMap = (std::unordered_map<eScriptTypes, Scriptable*>*)obj;

                    for (auto& scriptTypeScriptablePair : *scriptsMap)
                    {
                        if (!objTypeInfo->collectionTypeInfo)
                        {
                            LOG_ERROR("{0} Null collection type!", __FUNCTION__);
                            break;
                        }

                        SerializeToJson((void*)&scriptTypeScriptablePair, objTypeInfo->collectionTypeInfo, collectionJsonContainer);
                    }
                }
                break;

            case MirrorTypes::m_map_guid_entt:
                {
                    // #NOTE m_map_guid_entt type can only be owned by a Scene. Look at enforcing through some static asserts of runtime cast checks
                    const std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)obj;

                    const Mirror::TypeInfo* sceneInfo = Mirror::InfoForType<Scene>();

                    const std::string memberName = "m_GuidsToEntts";
                    u8 m_GuidsToEntts_Offset = INT8_MAX;
                    for (size_t i = 0; i < sceneInfo->fields.size(); i++)
                    {
                        const Mirror::Field& field = sceneInfo->fields[i];
                        if (field.name == memberName)
                        {
                            m_GuidsToEntts_Offset = field.offset;
                            break;
                        }
                    }

                    if (INT8_MAX == m_GuidsToEntts_Offset)
                    {
                        LOG_CRITICAL("{0} Could not serialize type {1} {2}!", __FUNCTION__, objTypeInfo->stringName, (u8)objTypeInfo->enumType);
                        break;
                    }

                    // #TODO Fix ugly special case requiring knowledge of owning type.
                    Scene* scene = (Scene*)((char*)obj - m_GuidsToEntts_Offset);

                    for (auto& entityPair : *entitiesMap)
                    {
                        EntityHandle handle(scene, entityPair.second);

                        // #TODO Special case where GUID needs to be known before object creation,
                        // So create an array with the GUID as the name. Review for better solution.
                        cJSON* entityJsonArray = CreateArray(std::to_string(handle.EntityGuid()).c_str());
                        AddItemToArray(collectionJsonContainer, entityJsonArray);

                        // #TODO Look to see how Hazel uses a list of types to simplify iteration such as below...
                        // Example in Components.h :
                        // template<typename... Component>
                        // struct ComponentGroup { };
                        //
                        // using AllComponents =
                        //     ComponentGroup<TransformComponent, SpriteRendererComponent,
                        //     CircleRendererComponent, CameraComponent, ScriptComponent,
                        //     NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent,
                        //     CircleCollider2DComponent, TextComponent>;
                        //
                        // See RegisterComponent(ComponentGroup<Component...>) in ScriptGlue.cpp
                        SerializeComponent<ComponentCamera>(handle, entityJsonArray);
                        SerializeComponent<ComponentInfo>(handle, entityJsonArray);
                        SerializeComponent<ComponentMesh>(handle, entityJsonArray);
                        SerializeComponent<ComponentTransform>(handle, entityJsonArray);
                        SerializeComponent<ComponentScript>(handle, entityJsonArray);
                    }
                }
                break;

            case MirrorTypes::m_floatArray16:
                {
                    const float* floatArray = (float*)obj;
                    for (size_t i = 0; i < 16; i++)
                    {
                        local_SerializePrimitive(&floatArray[i], objTypeInfo->collectionTypeInfo, collectionJsonContainer, objTypeInfo->stringName);
                    }
                }
                break;

            case MirrorTypes::m_vector_string: // #TODO Try to remove this case to use if (!objTypeInfo->fields.empty()) for loop
                {
                    const std::vector<std::string>* strings = (std::vector<std::string>*)obj;

                    for (size_t i = 0; i < strings->size(); i++)
                    {
                        local_SerializePrimitive(&strings[i], objTypeInfo->collectionTypeInfo, collectionJsonContainer, objTypeInfo->stringName);
                    }
                }
                break;

            case MirrorTypes::m_umap_guid_editorWindowPtr:
                {
                    const std::unordered_map<GUID, Editor::EditorWindow*>* guidEditorWindowPairs = (std::unordered_map<GUID, Editor::EditorWindow*>*)obj;

                    for (auto& pair : *guidEditorWindowPairs)
                    {
                        if (!pair.second)
                            continue;

                        const Mirror::TypeInfo* editorWindowTypeInfo = nullptr;
                        Editor::EditorWindowTypes editorWindowType = pair.second->Type();

                        switch (editorWindowType)
                        {
                            // Unique
                        case Editor::EditorWindowTypes::DefaultDebug:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowDefaultDebug>(); break;
                        case Editor::EditorWindowTypes::DockingContext:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowDefaultDebug>(); break;
                        case Editor::EditorWindowTypes::ImGuiDemo:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowImGuiDemo>(); break;
                        case Editor::EditorWindowTypes::MenuBar:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowMenuBar>(); break;
                            // Instanced
                        case Editor::EditorWindowTypes::Assets:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowAssets>(); break;
                        case Editor::EditorWindowTypes::EntityInspector:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowEntityInspector>(); break;
                        case Editor::EditorWindowTypes::SceneControls:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowSceneControls>(); break;
                        case Editor::EditorWindowTypes::SceneGraph:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowSceneGraph>(); break;
                        case Editor::EditorWindowTypes::SceneView:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowSceneView>(); break;
                        case Editor::EditorWindowTypes::Settings:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowSettings>(); break;
                        case Editor::EditorWindowTypes::StylePicker:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowStylePicker>(); break;
                        case Editor::EditorWindowTypes::MaterialEditor:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowMaterialEditor>(); break;
                        case Editor::EditorWindowTypes::FolderViewer:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowFolderViewer>(); break;
                        case Editor::EditorWindowTypes::NodeEditor:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowNodeEditor>(); break;
                        case Editor::EditorWindowTypes::ShaderEditor:
                            editorWindowTypeInfo = Mirror::InfoForType<Editor::EditorWindowShaderEditor>(); break;

                        case Editor::EditorWindowTypes::EditorWindowTypesInvalid:
                        default:
                            LOG_WARN("{0} Unsupported EditorWindowTypes value {1}", __FUNCTION__, (u8)editorWindowType);
                            break;
                        }

                        if (editorWindowTypeInfo)
                        {
                            cJSON* editorWindowJson = CreateArray((std::to_string((u32)pair.second->Type())).c_str());
                            if (editorWindowTypeInfo->isSubClass() && editorWindowTypeInfo->superTypeInfo)
                            {
                                // SerializeObjectToJson((void*)pair.second, editorWindowTypeInfo->superTypeInfo, editorWindowJson);
                            }

                            SerializeObjectToJson((void*)pair.second, editorWindowTypeInfo, editorWindowJson);
                            AddItemToArray(objJson, editorWindowJson);
                        }
                    }
                }
                break;

            case MirrorTypes::m_vec_pair_guid_string:
                {
                    using PairGuidString = std::pair<GUID, std::string>;
                    const std::vector<PairGuidString>* vecGuidStrings = (std::vector<PairGuidString>*)((char*)obj);

                    for (size_t i = 0; i < vecGuidStrings->size(); i++)
                    {
                        const PairGuidString& pair = vecGuidStrings->at(i);

                        const Mirror::TypeInfo* pairTypeInfo = Mirror::InfoForType<PairGuidString>();
                        SerializeToJson((void*)&pair, pairTypeInfo, collectionJsonContainer);
                    }
                }
                break;

            default:
                LOG_WARN("{0} Unsupported user defined field type {1}({2}) for serialization!", __FUNCTION__, objTypeInfo->stringName, (u32)objTypeInfo->enumType);
                break;
            }
        }

        // #define cJSON_Invalid (0)
        // #define cJSON_False  (1 << 0)
        // #define cJSON_True   (1 << 1)
        // #define cJSON_NULL   (1 << 2)
        // #define cJSON_Number (1 << 3)
        // #define cJSON_String (1 << 4)
        // #define cJSON_Array  (1 << 5)
        // #define cJSON_Object (1 << 6)
        // #define cJSON_Raw    (1 << 7) /* raw json */

        template <typename T>
        cJSON* TestCreateBool(const std::string& name, const void* obj)
        {
            return CreateBool(name.c_str(), *(T)obj);
        }

        template <typename T>
        cJSON* TestCreateNumber(const std::string& name, const void* obj)
        {
            return CreateNumber(name.c_str(), *(T)obj);
        }

        template <typename T>
        cJSON* TestCreateString(const std::string& name, const void* obj)
        {
            return CreateString(name.c_str(), (T)obj); // #NOTE obj is not dereferenced
        }

        void local_SerializePrimitive(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
        {
            if (!obj || !objTypeInfo || !objJson)
            {
                LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
                return;
            }

            // #TODO Actually serialize a given primitive value.

            // #TODO Try using memcpy or simple copying or raw byte data, if possible, to avoid handlling individual types.
            // Use type offset and size info.

            // memcpy_s(dest, destSize, src, srcSize);

            const u8 sizeOfType = objTypeInfo->size;

            // #TODO Consider creating an empty item here, instead of in each enum case.
            // That would mean special logic like setting the cJSON type would be needed below,
            // or more switch/if logic here (later probably better).
            cJSON* cJsonItem = nullptr;

            if (const bool testing = false)
            {
                if (MirrorTypes::m_bool == objTypeInfo->enumType)
                {
                    const bool result = *(bool*)obj;
                    cJsonItem = CreateBool(name.c_str(), result); // #TODO Deprecate QC_cJson library
                }
                else if (8 > sizeOfType) // 2 or 4 bytes
                {
                    cJsonItem = CreateNumber(name.c_str(), 0.);
                    memcpy((void*)&cJsonItem->valueint, obj, sizeOfType); // #TODO Pick one value to use
                    memcpy((void*)&cJsonItem->valuedouble, obj, sizeOfType);
                }
                else if (8 == sizeOfType)
                {
                    cJsonItem = CreateNumber(name.c_str(), 0.);
                    memcpy((void*)&cJsonItem->valuedouble, obj, sizeOfType);
                }
                else
                {
                    // Write to string?

                    // uint64_t* numberAddress = (uint64_t*)obj;
                    // AddItemToArray(objJson, CreateString(name.c_str(), std::to_string(*numberAddress).c_str()));
                }
            }

            switch (objTypeInfo->enumType)
            {
                case MirrorTypes::m_string:
                    {
                        const std::string* fieldAddress = (const std::string*)obj;
                        cJsonItem = CreateString(name.c_str(), fieldAddress->data()); // #TODO Requires ->data() so can't work with TestCreateString()
                    }
                    break;

                case MirrorTypes::eKeys:
                case MirrorTypes::m_char: // #TODO Note that const is forced here
                case MirrorTypes::m_charPtr:
                case MirrorTypes::m_constCharPtr:
                    cJsonItem = TestCreateString<const char*>(name, obj);
                    break;

                case MirrorTypes::m_bool:
                    cJsonItem = TestCreateBool<bool*>(name, obj);
                    break;

                case MirrorTypes::m_float:
                    cJsonItem = TestCreateNumber<float*>(name, obj);
                    break;

                case MirrorTypes::EditorWindowFlags:
                case MirrorTypes::EditorWindowTypes:
                case MirrorTypes::eScriptTypes: // #TODO Add a case for all enums by default
                case MirrorTypes::m_eSceneTypes:
                case MirrorTypes::m_uint8_t:
                    cJsonItem = TestCreateNumber<uint8_t*>(name, obj);
                    break;

                case MirrorTypes::m_uint16_t:
                    cJsonItem = TestCreateNumber<uint16_t*>(name, obj);
                    break;

                case MirrorTypes::m_uint32_t:
                    cJsonItem = TestCreateNumber<uint32_t*>(name, obj);
                    break;

                case MirrorTypes::m_int8_t:
                    cJsonItem = TestCreateNumber<int8_t*>(name, obj);
                    break;

                case MirrorTypes::m_int16_t:
                    cJsonItem = TestCreateNumber<int16_t*>(name, obj);
                    break;

                case MirrorTypes::m_int:
                case MirrorTypes::m_int32_t:
                    cJsonItem = TestCreateNumber<int32_t*>(name, obj);
                    break;

                case MirrorTypes::m_int64_t:
                    cJsonItem = TestCreateNumber<int64_t*>(name, obj);
                    break;

                case MirrorTypes::m_uint64_t: // #NOTE Special case of conversion on 64 bit types
                    {
                        // Use string instead of a double to avoid conversion issues
                        uint64_t* numberAddress = (uint64_t*)obj;
                        cJsonItem = CreateString(name.c_str(), std::to_string(*numberAddress).c_str());
                    }
                    // cJsonItem = TestCreateNumber<uint64_t*>(name, obj);
                    break;

                case MirrorTypes::m_double:
                    cJsonItem = TestCreateNumber<double*>(name, obj);
                    break;

                default:
                    LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, name, objTypeInfo->stringName, (int)objTypeInfo->enumType);
                    break;
            }

            if (cJsonItem)
            {
                AddItemToObject(objJson, cJsonItem);
            }
        }

    }

}
