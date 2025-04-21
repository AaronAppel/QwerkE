#include "QE_EditorInspector.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_Buffer.h"
#include "QF_ComponentScript.h"
#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Scenes.h"

#include "QF_ComponentMesh.h"

#include "QF_Scriptable.h"
#include "QE_Settings.h"

#include "QE_Mirror.h"

namespace QwerkE {

    namespace Inspector {

        // #TESTING
        struct InspectFieldReturn
        {
            std::string selectedFieldName;
        };

        bool local_InspectPrimitive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        InspectFieldReturn local_InspectClassFields(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        bool local_InspectClass(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        bool local_InspectCollection(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        bool local_InspectPair(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);

        // #TODO Consider adding a read only bool argument
        // #TODO Look to make parentName a const&
        bool InspectType(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            bool valueChanged = false;

            switch (typeInfo->category)
            {
                case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
                    valueChanged |= local_InspectPrimitive(typeInfo, obj, parentName);
                    break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
                    valueChanged |= local_InspectClass(typeInfo, obj, parentName);
                    break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
                    valueChanged |= local_InspectCollection(typeInfo, obj, parentName);
                    break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
                    valueChanged |= local_InspectPair(typeInfo, obj, parentName);
                    break;

                case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
                    // #TODO Modify parentName?
                    valueChanged |= InspectType(typeInfo->AbsoluteType(), *(void**)obj, parentName);
                    break;
            }

            return valueChanged;
        }

        static bool hasWarned = false;
        bool local_InspectPrimitive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            if (!hasWarned && Mirror::TypeInfoCategory_Primitive != typeInfo->category)
            {
                LOG_WARN("{0} Invalid type {1} {2}({3}) for primitive inspection", __FUNCTION__, parentName, typeInfo->stringName.c_str(), (int)typeInfo->id);
                hasWarned = true;
            }

            bool valueChanged = false;

#ifdef _QDEARIMGUI

            std::string elementName = parentName;

            // #TODO Try to use isDraggingPayload to avoid extra value changes when using a slider as a payload source
            const bool isDraggingPayload = nullptr != ImGui::GetDragDropPayload();

            switch (typeInfo->id)
            {
            case Mirror::TypeId<std::string>():
                {
                    std::string* stringAddress = (std::string*)obj;

                    std::string buffer;
                    buffer.reserve(INT8_MAX);
                    buffer = *stringAddress;

                    if (ImGui::InputText(elementName.c_str(), buffer.data(), buffer.capacity()))
                    {
                        valueChanged |= *stringAddress != buffer;
                        *stringAddress = buffer;
                    }
                }
                break;

            case Mirror::TypeId<char*>():
            case Mirror::TypeId<const char*>():
                {
                    const char** constCharPtrAddress = (const char**)obj;

                    std::string buffer;
                    buffer.reserve(INT8_MAX);
                    buffer = *constCharPtrAddress;

                    if (ImGui::InputText(elementName.c_str(), buffer.data(), buffer.capacity()))
                    {
                        auto oldData = constCharPtrAddress;

                        char* result = strdup(buffer.data());
                        valueChanged |= strcmp(*constCharPtrAddress, result) != 0;
                        *constCharPtrAddress = result;

                        // delete oldData; // #TODO Review memory leak. delete *constCharPtrAddress;
                    }
                }
                break;

            case Mirror::TypeId<char>():
            case Mirror::TypeId<eKeys>():
                {
                    char* charPtrAddress = (char*)obj;
                    char charEscaped[2] = { *charPtrAddress, '\0' };
                    if (ImGui::InputText(elementName.c_str(), charEscaped, 2))
                    {
                        // #TODO Create constants or use isAlpha, isDigit helpers
                        if (charEscaped[0] >= 97 && charEscaped[0] <= 122)
                        {
                            charEscaped[0] -= 32;
                        }
                        valueChanged |= *charPtrAddress != charEscaped[0];
                        *charPtrAddress = charEscaped[0];
                    }
                }
                break;

            case Mirror::TypeId<bool>():
                if (ImGui::Checkbox(elementName.c_str(), (bool*)obj)) { valueChanged = true; } break;
            case Mirror::TypeId<float>():
                if (ImGui::DragFloat(elementName.c_str(), (float*)obj, .1f)) { valueChanged = true; } break;

            case Mirror::TypeId<double>():
            {
                double* numberAddress = (double*)obj;
                float temp = (float)*numberAddress;

                if (ImGui::DragFloat(elementName.c_str(), &temp)) // #NOTE Loss of precision
                {
                    valueChanged |= *numberAddress != (double)temp;
                    *numberAddress = (double)temp;
                }
            }
            break;

            case Mirror::TypeId<u8>():
                {
                    u8* numberAddress = (u8*)obj;
                    int temp = (int)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp), U8_MIN, U8_MAX)
                    {
                        valueChanged |= *numberAddress != (u8)temp;
                        *numberAddress = (u8)temp;
                    }
                }
                break;

            case Mirror::TypeId<u16>():
                {
                    u16* numberAddress = (u16*)obj;
                    int temp = (int)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp), U16_MIN, U16_MAX)
                    {
                        valueChanged |= *numberAddress != (u16)temp;
                        *numberAddress = (u16)temp;
                    }
                }
                break;

            case Mirror::TypeId<u32>():
                {
                    u32* numberAddress = (u32*)obj;
                    s32 temp = (s32)*numberAddress;

                    // #TODO Change u64 solution of using a string for a larger value range
                    if (ImGui::DragInt(elementName.c_str(), &temp), U32_MIN, U32_MAX)
                    {
                        valueChanged |= *numberAddress != (u32)temp; // #NOTE Signed to unsigned conversion
                        *numberAddress = (u32)temp;
                    }
                }
                break;

            case Mirror::TypeId<u64>():
                {
                    u64* numberAddress = (u64*)obj;

                    char buffer[U64_MAX_DIGITS] = { '\0' };
                    strcpy(buffer, std::to_string(*numberAddress).c_str());

                    ImGui::PushItemWidth(165);
                    if (ImGui::InputText(elementName.c_str(), buffer, U64_MAX_DIGITS))
                    {
                        if (buffer[0] != '\0')
                        {
                            *numberAddress = std::stoull(buffer); // #NOTE stoull crashes on empty string
                        }
                        else
                        {
                            *numberAddress = '\0';
                        }
                        valueChanged = true;
                    }
                    ImGui::PopItemWidth();

                    std::string popUpName = "Context ";
                    popUpName += elementName;
                    if (ImGui::IsItemClicked(ImGui::MouseRight))
                    {
                        ImGui::OpenPopup(popUpName.c_str());
                    }

                    if (ImGui::BeginPopup(popUpName.c_str()))
                    {
                        if (ImGui::Button(("Paste From Clipboard##" + elementName).c_str()))
                        {
                            std::string clipBoardText = ImGui::GetClipboardText();
                            LOG_INFO("{0} Pasting from clipboard: {1}", __FUNCTION__, clipBoardText.c_str());
                            *numberAddress = std::stoull(clipBoardText.c_str());
                            valueChanged = true;
                        }
                        if (ImGui::Button(("Copy To Clipboard##" + elementName).c_str()))
                        {
                            std::string numberAsString = buffer;
                            ImGui::SetClipboardText(numberAsString.c_str());
                            LOG_INFO("{0} Copied to clipboard: {1}", __FUNCTION__, numberAsString.c_str());
                        }
                        if (ImGui::Button(("Regenerate GUID##" + elementName).c_str()))
                        {
                            *numberAddress = GUID();
                            valueChanged = true;
                            LOG_INFO("{0} Regenerated GUID: {1}", __FUNCTION__, *numberAddress);
                        }
                        ImGui::EndPopup();
                    }
                }
                break;

            case Mirror::TypeId<s8>():
                {
                    s8* numberAddress = (s8*)obj;
                    s32 temp = (s32)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp), 0, S8_MAX)
                    {
                        valueChanged |= *numberAddress != (s8)temp;
                        *numberAddress = (s8)temp;
                    }
                }
                break;

            case Mirror::TypeId<s16>():
                {
                    s16* numberAddress = (s16*)obj;
                    s32 temp = (s32)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp), 0, S16_MAX)
                    {
                        valueChanged |= *numberAddress != (s16)temp;
                        *numberAddress = (s16)temp;
                    }
                }
                break;

            case Mirror::TypeId<s32>():
                if (ImGui::DragInt(elementName.c_str(), (s32*)obj), S32_MIN, S32_MAX) { valueChanged = true; } break;
            case Mirror::TypeId<s64>():
                {
                    // #TODO Change u64 solution of using a string for a larger value range
                    s64* numberAddress = (s64*)obj;
                    s32 temp = (s32)*numberAddress;

                    if (ImGui::DragInt(elementName.c_str(), &temp), S64_MIN, S64_MAX)
                    {
                        valueChanged |= *numberAddress != (s64)temp; // #TODO Fix 32 bit to 64 bit conversion
                        *numberAddress = (s64)temp;
                    }
                }
                break;

            default:
                if (!hasWarned)
                {
                    LOG_WARN("{0} Unsupported field type {1} {2}({3}) for inspection", __FUNCTION__, elementName, typeInfo->stringName.c_str(), (int)typeInfo->id);
                    hasWarned = true;
                }
                break;
            }

            // #TODO Consider moving to a new function to keep this function smaller and specific to interactable elements
            // Drag and drop behaviour
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload(typeInfo->stringName.c_str(), obj, typeInfo->size);
                ImGui::EndDragDropSource();
            }

            // #TODO Get field flags to know if drag target/source and drop is supported
            // typeInfo->field[i].flags

            const int fieldFlag = 1;
            if (fieldFlag | FieldSerializationFlags::_InspectorDisableDragAndDropTarget &&
                ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(typeInfo->stringName.c_str()))
                {
                    memcpy(obj, payload->Data, typeInfo->size);
                }
                ImGui::EndDragDropTarget();
            }

            if (ImGui::IsItemClicked())
            {
                int bp = 0;
            }

#endif // #if _QDEARIMGUI
            return valueChanged;
        }

        InspectFieldReturn local_InspectClassFields(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            InspectFieldReturn returnValue;

            for (size_t i = 0; i < typeInfo->fields.size(); i++)
            {
                const Mirror::Field field = typeInfo->fields[i];
                if (field.flags & FieldSerializationFlags::_HideInInspector)
                    continue;

                ImGui::BeginDisabled(field.flags & FieldSerializationFlags::_InspectorViewOnly);

                std::string fieldName = parentName + field.name + "##";
                void* fieldAddress = (char*)obj + field.offset;

                // #TODO May need to pass flags from here, at least for drag and drop payloads
                if (InspectType(field.typeInfo, fieldAddress, fieldName))
                {
                    returnValue.selectedFieldName = field.name;
                }
                else if (ImGui::IsItemClicked(ImGui::MouseLeft))
                {
                    returnValue.selectedFieldName = field.name;
                }

                ImGui::EndDisabled();
            }
            return returnValue;
        }

        bool local_InspectClass(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            bool valueChanged = false;
            std::string elementName = parentName;

            switch (typeInfo->id)
            {
            case Mirror::TypeId<vec2f>():
                if (ImGui::DragFloat2(elementName.c_str(), (float*)obj, .1f)) { valueChanged = true; } break;
            case Mirror::TypeId<vec3f>():
                if (ImGui::DragFloat3(elementName.c_str(), (float*)obj, .1f)) { valueChanged = true; } break;
            case Mirror::TypeId<QwerkE::ScriptGuiButton>():
                {
                    ScriptGuiButton* button = (ScriptGuiButton*)obj;

                    std::string elementName = button->m_ButtonName + "##" + parentName;
                    if (ImGui::Button(elementName.c_str()) && button->m_CallbackFunction)
                    {
                        button->m_CallbackFunction();
                    }
                }
                break;

            // Overrides for on change callback or on interaction special behaviour
            case Mirror::TypeId<ComponentMesh>():
                {
                    const char* popUpNameMeshes = "ComponentMeshPopUpMeshSelection";
                    const char* popUpNameShaders = "ComponentMeshPopUpShaderSelection";

                    // #TODO Address redundancy and safety of automatic mesh data re-intialization
                    if (ImGui::Button("Initialize"))
                    {
                        ComponentMesh* mesh = (ComponentMesh*)obj;
                        mesh->Initialize();
                    }

                    InspectFieldReturn result = local_InspectClassFields(typeInfo, obj, parentName);
                    if (!result.selectedFieldName.empty())
                    {
                        // #TODO Detect member field selection (pop up)
                        if (result.selectedFieldName == "m_MeshGuid")
                        {
                            ImGui::OpenPopup(popUpNameMeshes);
                        }
                        else if (result.selectedFieldName == "m_ShaderGuid")
                        {
                            ImGui::OpenPopup(popUpNameShaders);
                        }
                    }

                    if (ImGui::BeginPopup(popUpNameMeshes))
                    {
                        // const std::unordered_map<GUID, bgfxFramework::Mesh*>& meshes = Assets::ViewAssets<bgfxFramework::Mesh>();
                        // for (auto& guidMeshPair : meshes)
                        // {
                        //     bool clicked = false;
                        //     ImGui::Text("GUID: ");
                        //     if (ImGui::IsItemClicked(ImGui::MouseLeft))
                        //     {
                        //         clicked = true;
                        //     }
                        //     ImGui::SameLine();
                        //     ImGui::Text(std::to_string(guidMeshPair.first).c_str());
                        //     if (ImGui::IsItemClicked(ImGui::MouseLeft))
                        //     {
                        //         clicked = true;
                        //     }
                        //
                        //     if (clicked)
                        //     {
                        //         ComponentMesh* mesh = (ComponentMesh*)obj;
                        //         mesh->SetMeshGuid(guidMeshPair.first);
                        //         mesh->Initialize();
                        //         break;
                        //     }
                        // }

                        std::unordered_map<size_t, AssetsList>& assetRegistry = Assets::ViewRegistry();
                        if (assetRegistry.find(Mirror::TypeId<bgfxFramework::Mesh>()) != assetRegistry.end())
                        {
                            auto meshes = assetRegistry[Mirror::TypeId<bgfxFramework::Mesh>()];

                            for (auto& guidMeshPair : meshes)
                            {
                                bool clicked = false;
                                ImGui::Text("GUID: ");
                                if (ImGui::IsItemClicked(ImGui::MouseLeft))
                                {
                                    clicked = true;
                                }
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidMeshPair.first).c_str());
                                if (ImGui::IsItemClicked(ImGui::MouseLeft))
                                {
                                    clicked = true;
                                }

                                if (clicked)
                                {
                                    ComponentMesh* meshComp = (ComponentMesh*)obj;
                                    meshComp->SetMeshGuid(guidMeshPair.first);
                                    meshComp->Initialize();
                                    break;
                                }
                            }
                        }

                        ImGui::EndPopup();
                    }
                    else if (ImGui::BeginPopup(popUpNameShaders))
                    {
                        // const std::unordered_map<GUID, Shader*>& shaders = Assets::ViewAssets<Shader>();
                        // for (auto& guidShaderPair : shaders)
                        // {
                        //     bool clicked = false;
                        //     ImGui::Text("GUID: ");
                        //     if (ImGui::IsItemClicked(ImGui::MouseLeft))
                        //     {
                        //         clicked = true;
                        //     }
                        //     ImGui::SameLine();
                        //     ImGui::Text(std::to_string(guidShaderPair.first).c_str());
                        //     if (ImGui::IsItemClicked(ImGui::MouseLeft))
                        //     {
                        //         clicked = true;
                        //     }
                        //
                        //     if (clicked)
                        //     {
                        //         ComponentMesh* meshComp = (ComponentMesh*)obj;
                        //         meshComp->SetShaderGuid(guidShaderPair.first);
                        //         meshComp->Initialize();
                        //         break;
                        //     }
                        // }

                        std::unordered_map<size_t, AssetsList>& assetRegistry = Assets::ViewRegistry();
                        if (assetRegistry.find(Mirror::TypeId<Shader>()) != assetRegistry.end())
                        {
                            auto shaders = assetRegistry[Mirror::TypeId<Shader>()];

                            for (auto& guidShaderPair : shaders)
                            {
                                bool clicked = false;
                                ImGui::Text("GUID: ");
                                if (ImGui::IsItemClicked(ImGui::MouseLeft))
                                {
                                    clicked = true;
                                }
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidShaderPair.first).c_str());
                                if (ImGui::IsItemClicked(ImGui::MouseLeft))
                                {
                                    clicked = true;
                                }

                                if (clicked)
                                {
                                    ComponentMesh* meshComp = (ComponentMesh*)obj;
                                    meshComp->SetShaderGuid(guidShaderPair.first);
                                    meshComp->Initialize();
                                    break;
                                }
                            }
                        }

                        ImGui::EndPopup();
                    }

                    valueChanged |= !result.selectedFieldName.empty();
                }
                break;

            // Fall through statements (no break)

            default:
                // #TODO ComponentScript fields for scripts like ScriptablePatrol are not unique
                // Look at why the string is incorrect
                valueChanged |= !local_InspectClassFields(typeInfo, obj, parentName).selectedFieldName.empty();
                break;
            }

            return valueChanged;
        }

        bool local_InspectCollection(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            bool valueChanged = false;

            std::string elementName = parentName;
            switch (typeInfo->id)
            {
            case Mirror::TypeId<std::unordered_map<eScriptTypes, Scriptable*>>():
                {
                    std::unordered_map<eScriptTypes, Scriptable*>* scriptsMap = (std::unordered_map<eScriptTypes, Scriptable*>*)obj;

                    if (ImGui::Button("+"))
                    {
                        ImGui::OpenPopup("ScriptList");
                    }

                    if (ImGui::BeginPopup("ScriptList"))
                    {
                        ComponentScript* script = (ComponentScript*)obj;

                        Scene* scene = Scenes::GetCurrentScene();

                        EntityHandle entity = EntityHandle::InvalidHandle();
                        for (auto& pair : scene->EntitiesMap())
                        {
                            EntityHandle tempEntity = EntityHandle(scene, pair.first);

                            if (tempEntity.HasComponent<ComponentScript>() &&
                                &tempEntity.GetComponent<ComponentScript>() == script)
                            {
                                entity = tempEntity;
                                break;
                            }
                        }

                        if (entity)
                        {
                            constexpr u8 start = (u8)eScriptTypes::Invalid + 1;
                            constexpr u8 range = (u8)eScriptTypes::COUNT;

                            for (size_t i = start; i < range; i++)
                            {
                                eScriptTypes scriptType = (eScriptTypes)i;
                                if (!script->HasScript(scriptType) && ImGui::Button(ENUM_TO_STR(eScriptTypesStr::_from_index((u8)i))))
                                {
                                    script->AddScript(scriptType, entity);
                                    valueChanged = true;
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        else
                        {
                            // #TODO Error
                        }

                        ImGui::EndPopup();
                    }

                    eScriptTypes removedScriptType = eScriptTypes::Invalid;
                    for (auto& pair : *scriptsMap)
                    {
                        ImGui::Text((std::to_string(pair.first) + " ").c_str());
                        ImGui::SameLine();
                        ImGui::Text(ENUM_TO_STR(eScriptTypesStr::_from_index((u8)pair.first)));

                        char minusButtonTitle[] = { '-', '#', '#', (char)pair.first, '\0' };
                        ImGui::SameLineEnd(1);
                        if (ImGui::Button(minusButtonTitle)) // #TODO Review using parentName as well
                        {
                            removedScriptType = pair.first;
                        }

                        // #TODO See how ComponentScriptsList can help reduce future script creation steps
                        const Mirror::TypeInfo* scriptableTypeInfo = nullptr;
                        switch (pair.first)
                        {
                        case eScriptTypes::Patrol:
                            scriptableTypeInfo = Mirror::InfoForType<ScriptablePatrol>(); break;
                        case eScriptTypes::PathFinder:
                            scriptableTypeInfo = Mirror::InfoForType<ScriptablePathFinder>(); break;
                        case eScriptTypes::Camera:
                            scriptableTypeInfo = Mirror::InfoForType<ScriptableCamera>(); break;
                        case eScriptTypes::Testing:
                            scriptableTypeInfo = Mirror::InfoForType<ScriptableTesting>(); break;
                        case eScriptTypes::SceneTransition:
                            scriptableTypeInfo = Mirror::InfoForType<ScriptableSceneTransition>(); break;
                        }

                        if (scriptableTypeInfo)
                        {
                            ScriptablePathFinder* scr = (ScriptablePathFinder*)pair.second;
                            valueChanged |= InspectType(scriptableTypeInfo, (void*)pair.second, parentName);
                        }
                    }

                    if (removedScriptType != eScriptTypes::Invalid)
                    {
                        ComponentScript* script = (ComponentScript*)obj;
                        script->RemoveScript(removedScriptType);
                        valueChanged = true;
                    }
                }
                break;

            case Mirror::TypeId<std::vector<std::string>>(): // #TODO Support vector manipulation
                {
                    std::vector<std::string>* strings = (std::vector<std::string>*)obj;
                    for (size_t i = 0; i < strings->size(); i++)
                    {
                        const std::string* stringAddress = (const std::string*)strings->data() + i;
                        elementName += std::to_string(i);

                        valueChanged |= InspectType(Mirror::InfoForType<std::string>(), &strings->at(i), elementName);
                    }
                }
                break;

            case Mirror::TypeId<std::vector<std::string*>>():
                {
                    std::vector<std::string>* strings = *(std::vector<std::string>**)obj;
                    for (size_t i = 0; i < strings->size(); i++)
                    {
                        const std::string* stringAddress = (const std::string*)strings->data() + i;
                        elementName += "_" + std::to_string(i);

                        valueChanged |= InspectType(Mirror::InfoForType<std::string>(), &strings->at(i), elementName);
                    }
                }
                break;

            case Mirror::TypeId<float[16]>():
                {
                    float* f16matrix = (float*)obj;
                    valueChanged |= ImGui::DragFloat4(elementName.c_str(), f16matrix, .1f);
                    valueChanged |= ImGui::DragFloat4("##m_arr_float16_1", &f16matrix[4], .1f);
                    valueChanged |= ImGui::DragFloat4("##m_arr_float16_2", &f16matrix[8], .1f);
                    valueChanged |= ImGui::DragFloat4("##m_arr_float16_3", &f16matrix[12], .1f);
                }
                break;

            default:
                // #TODO Inspect collection elements

                break;
            }
            return valueChanged;
        }

        bool local_InspectPair(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            bool valueChanged = false;

            std::string elementName = parentName;
            // switch (typeInfo->id)
            // {
            // default:
            //     break;
            // }
            return valueChanged;
        }

    }

}
