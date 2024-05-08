#include "QE_EditorInspector.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_ComponentScript.h"
#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Scenes.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"
#include "QF_ScriptPathFinder.h"
#include "QF_ScriptPatrol.h"
#include "QF_ScriptTesting.h"
#include "QE_Settings.h"

namespace QwerkE {

    namespace Inspector {

        static bool hasWarned = false;
        bool InspectFieldRecursive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            bool valueChanged = false;

#ifdef _QDEARIMGUI

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    std::filesystem::path texturePath(path);
                }
                ImGui::EndDragDropTarget();
            }

            const std::vector<Mirror::Field>& fields = typeInfo->fields;
            for (size_t i = 0; i < fields.size(); i++)
            {
                const Mirror::Field field = fields[i];
                if (field.serializationFlags & Mirror::FieldSerializationFlags::_HideInInspector)
                    continue;

                if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorViewOnly)
                {
                    // #TODO Show but prevent editing
                }

                std::string fieldName = field.name + "##" + parentName;
                void* fieldAddress = (char*)obj + field.offset;
                valueChanged |= InspectFieldRecursive(field.typeInfo, fieldAddress, fieldName);
            }

            std::string elementName = parentName;
            switch (typeInfo->enumType)
            {
            case MirrorTypes::m_vec_string_ptr:
                {
                    std::vector<std::string>* strings = *(std::vector<std::string>**)obj;
                    for (size_t i = 0; i < strings->size(); i++)
                    {
                        const std::string* stringAddress = (const std::string*)strings->data() + i;
                        elementName += "_" + std::to_string(i);

                        valueChanged |= InspectFieldRecursive(Mirror::InfoForType<std::string>(), &strings->at(i), elementName);
                    }
                }
                break;

            case MirrorTypes::ScriptGuiButton:
                {
                    ScriptGuiButton* button = (ScriptGuiButton*)obj;

                    std::string elementName = button->m_ButtonName + "##" + parentName;
                    if (ImGui::Button(elementName.c_str()) && button->m_CallbackFunction)
                    {
                        button->m_CallbackFunction();
                    }
                }
                break;

            case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
                {
                    if (typeInfo->enumType != MirrorTypes::ComponentScript)
                    {
                        // LOG_ERROR("{0} Expected different parent type!", __FUNCTION__);
                        // break;
                    }

                    std::unordered_map<eScriptTypes, Scriptable*>* scriptsMap = (std::unordered_map<eScriptTypes, Scriptable*>*)obj;

                    if (ImGui::Button("+"))
                    {
                        ImGui::OpenPopup("ScriptList");
                    }

                    if (ImGui::BeginPopup("ScriptList"))
                    {
                        ComponentScript* script = (ComponentScript*)obj;

                        const u8 start = (u8)eScriptTypes::Invalid + 1;
                        const u8 range = (u8)eScriptTypes::COUNT;
                        for (size_t i = start; i < range; i++)
                        {
                            eScriptTypes scriptType = (eScriptTypes)i;
                            if (!script->HasScript(scriptType) && ImGui::Button(ENUM_TO_STR(eScriptTypesStr::_from_index((u8)i))))
                            {
                                script->AddScript(scriptType, EntityHandle::InvalidHandle()); // #TODO Get proper entity handle
                                valueChanged = true;
                                ImGui::CloseCurrentPopup();
                            }
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
                        if (ImGui::Button(minusButtonTitle))
                        {
                            removedScriptType = pair.first;
                        }

                        switch (pair.first)
                        {
                        case eScriptTypes::Camera:
                            break;

                        case eScriptTypes::Patrol:
                        {
                            const Mirror::TypeInfo* scriptableTypeInfo = Mirror::InfoForType<ScriptablePatrol>();
                            ScriptablePatrol* scr = (ScriptablePatrol*)pair.second;
                            valueChanged |= InspectFieldRecursive(scriptableTypeInfo, (void*)pair.second, parentName);
                        }
                        break;

                        case eScriptTypes::Testing:
                            break;

                        case eScriptTypes::PathFinder:
                        {
                            const Mirror::TypeInfo* scriptableTypeInfo = Mirror::InfoForType<ScriptablePathFinder>();
                            ScriptablePathFinder* scr = (ScriptablePathFinder*)pair.second;
                            valueChanged |= InspectFieldRecursive(scriptableTypeInfo, (void*)pair.second, parentName);
                        }
                        break;

                        default:
                            // ASSERT(false, "Unsupported script type!");
                            break;
                        }
                    }

                    if (removedScriptType != eScriptTypes::Invalid)
                    {
                        ComponentScript* script = (ComponentScript*)obj;
                        script->RemoveScript(removedScriptType);
                        // #TODO Look to remove script
                        valueChanged = true;
                    }
                }
                break;

            case MirrorTypes::m_arr_float16:
                {
                    float* f16matrix = (float*)obj;
                    valueChanged |= ImGui::DragFloat4(elementName.c_str(), f16matrix, .1f);
                    valueChanged |= ImGui::DragFloat4("##1", &f16matrix[4], .1f);
                    valueChanged |= ImGui::DragFloat4("##2", &f16matrix[8], .1f);
                    valueChanged |= ImGui::DragFloat4("##3", &f16matrix[12], .1f);
                }
                break;

            case MirrorTypes::m_vec_string: // #TODO Support vector manipulation
                {
                    std::vector<std::string>* strings = (std::vector<std::string>*)obj;
                    for (size_t i = 0; i < strings->size(); i++)
                    {
                        const std::string* stringAddress = (const std::string*)strings->data() + i;
                        elementName += std::to_string(i);

                        valueChanged |= InspectFieldRecursive(Mirror::InfoForType<std::string>(), &strings->at(i), elementName);
                    }
                }
                break;

            case MirrorTypes::vec2f:
                {
                    float* vector2Address = (float*)obj;
                    if (ImGui::DragFloat2(elementName.c_str(), vector2Address, .1f))
                    {
                        valueChanged = true;
                    }
                }
                break;

            case MirrorTypes::vec3f:
                {
                    float* vector3Address = (float*)obj;
                    if (ImGui::DragFloat3(elementName.c_str(), vector3Address, .1f))
                    {
                        valueChanged = true;
                    }
                }
                break;

            case MirrorTypes::m_string:
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

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
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

                        //delete oldData; // #TODO Review memory leak. delete *constCharPtrAddress;
                    }
                }
                break;

            case MirrorTypes::m_char:
            case MirrorTypes::eKeys:
                {
                    char* charPtrAddress = (char*)obj;
                    char charEscaped[2] = { *charPtrAddress, '\0' };
                    if (ImGui::InputText(elementName.c_str(), charEscaped, 2))
                    {
                        if (charEscaped[0] >= 97 && charEscaped[0] <= 122)
                        {
                            charEscaped[0] -= 32;
                        }
                        valueChanged |= *charPtrAddress != charEscaped[0];
                        *charPtrAddress = charEscaped[0];
                    }
                }
                break;

            case MirrorTypes::m_bool:
                {
                    bool* boolAddress = (bool*)obj;
                    if (ImGui::Checkbox(elementName.c_str(), boolAddress))
                    {
                        valueChanged = true;
                    }
                }
                break;

            case MirrorTypes::m_float:
                {
                    float* numberAddress = (float*)obj;
                    if (ImGui::DragFloat(elementName.c_str(), numberAddress, .1f))
                    {
                        valueChanged = true;
                    }
                }
                break;


            case MirrorTypes::m_eSceneTypes:
            case MirrorTypes::m_uint8_t:
                {
                    u8* numberAddress = (u8*)obj;
                    int temp = (int)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (u8)temp;
                        *numberAddress = (u8)temp;
                    }
                }
                break;

            case MirrorTypes::m_uint16_t:
                {
                    u16* numberAddress = (u16*)obj;
                    int temp = (int)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (u16)temp;
                        *numberAddress = (u16)temp;
                    }
                }
                break;

            case MirrorTypes::m_uint32_t:
                {
                    uint32_t* numberAddress = (uint32_t*)obj;
                    int temp = (int)*numberAddress;

                    if (ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (uint32_t)temp;
                        *numberAddress = (uint32_t)temp;
                    }
                }
                break;

            case MirrorTypes::m_uint64_t:
                {
                    uint64_t* numberAddress = (uint64_t*)obj;

                    std::string numberAsString = std::to_string(*numberAddress);
                    if (ImGui::InputText(elementName.c_str(), numberAsString.data(), numberAsString.size(), ImGuiInputTextFlags_ReadOnly))
                    {
                        // #TODO Fix when cursor selects text box, the last character is deleted
                        *numberAddress = std::stoull(numberAsString.c_str());
                        valueChanged = true;
                    }

                    if (ImGui::IsItemClicked(ImGui::MouseRight))
                    {
                        ImGui::OpenPopup("CopyToClipboard");
                    }

                    if (ImGui::BeginPopup("CopyToClipboard"))
                    {
                        if (ImGui::Button("Copy To Clipboard"))
                        {
                            ImGui::SetClipboardText(numberAsString.c_str());
                            LOG_INFO("{0} Copied to clipboard: {1}", __FUNCTION__, numberAsString.c_str());
                        }
                        ImGui::EndPopup();
                    }

                    int temp = (int)*numberAddress;
                    if (false && ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (uint64_t)temp;
                        *numberAddress = (uint64_t)temp;
                    }
                }
                break;

            case MirrorTypes::m_int8_t:
                {
                    s8* numberAddress = (s8*)obj;
                    int temp = (int)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (s8)temp;
                        *numberAddress = (s8)temp;
                    }
                }
                break;

            case MirrorTypes::m_int16_t:
                {
                    s16* numberAddress = (s16*)obj;
                    int temp = (int)*numberAddress;
                    if (ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (s16)temp;
                        *numberAddress = (s16)temp;
                    }
                }
                break;

            case MirrorTypes::m_int32_t:
            case MirrorTypes::m_int:
                {
                    int* numberAddress = (int*)obj;
                    if (ImGui::DragInt(elementName.c_str(), numberAddress))
                    {
                        valueChanged = true;
                    }
                }
                break;

            case MirrorTypes::m_int64_t:
                {
                    int64_t* numberAddress = (int64_t*)obj;
                    int temp = (int)*numberAddress;

                    if (ImGui::DragInt(elementName.c_str(), &temp))
                    {
                        valueChanged |= *numberAddress != (int64_t)temp;
                        *numberAddress = (int64_t)temp;
                    }
                }
                break;

            case MirrorTypes::m_double:
                {
                    double* numberAddress = (double*)obj;
                    float temp = (float)*numberAddress;

                    if (ImGui::DragFloat(elementName.c_str(), &temp)) // #TODO Review need for precision
                    {
                        valueChanged |= *numberAddress != (double)temp;
                        *numberAddress = (double)temp;
                    }
                }
                break;

            default:
                if (!hasWarned)
                {
                    LOG_WARN("{0} Unsupported field type {1} {2}({3}) for inspection", __FUNCTION__, elementName, typeInfo->stringName.c_str(), (int)typeInfo->enumType);
                    hasWarned = true;
                }
                break;
            }
#endif
            return valueChanged;
        }

    }

}
