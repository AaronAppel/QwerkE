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
#include "QF_Settings.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"
#include "QF_ScriptPathFinder.h"
#include "QF_ScriptPatrol.h"
#include "QF_ScriptTesting.h"

namespace QwerkE {

    namespace Inspector {

        static bool hasWarned = false;
        bool InspectFieldRecursive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            bool valueChanged = false;

#ifdef _QDEARIMGUI
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

                const void* fieldAddress = (void*)((char*)obj + field.offset);

                switch (field.typeInfo->enumType)
                {
                case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
                    {
                        if (typeInfo->enumType != MirrorTypes::ComponentScript)
                        {
                            LOG_ERROR("{0} Expected different parent type!", __FUNCTION__);
                            break;
                        }

                        std::unordered_map<eScriptTypes, Scriptable*>* scriptsMap = (std::unordered_map<eScriptTypes, Scriptable*>*)fieldAddress;

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
                                ASSERT(false, "Unsupported script type!");
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

                case MirrorTypes::m_floatArray16:
                    {
                        float* f16matrix = (float*)fieldAddress;
                        std::string fieldName = parentName + field.name;
                        valueChanged |= ImGui::DragFloat4(fieldName.c_str(), f16matrix, .1f);
                        valueChanged |= ImGui::DragFloat4("##1", &f16matrix[4], .1f);
                        valueChanged |= ImGui::DragFloat4("##2", &f16matrix[8], .1f);
                        valueChanged |= ImGui::DragFloat4("##3", &f16matrix[12], .1f);
                    }
                    break;

                case MirrorTypes::m_vector_string: // #TODO Support vector manipulation
                    {
                        std::vector<std::string>* strings = (std::vector<std::string>*)fieldAddress;
                        for (size_t i = 0; i < strings->size(); i++)
                        {
                            const std::string* stringAddress = (const std::string*)strings->data() + i;
                            std::string fieldName = parentName + field.name + std::to_string(i);
                            ImGui::LabelText(fieldName.c_str(), stringAddress->data());
                            // #TODO Edit strings
                            // if ()
                            // {
                            //     valueChanged |= ImGui::InputText;
                            // }
                        }
                        break;
                    }
                    break;

                case MirrorTypes::vec2f:
                    {
                        float* vector2Address = (float*)fieldAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragFloat2(fieldName.c_str(), vector2Address, .1f))
                        {
                            valueChanged = true;
                        }
                    }
                    break;

                case MirrorTypes::vec3f:
                    {
                        float* vector3Address = (float*)fieldAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragFloat3(fieldName.c_str(), vector3Address, .1f))
                        {
                            valueChanged = true;
                        }
                    }
                    break;

                case MirrorTypes::m_string:
                    {
                        std::string* stringAddress = (std::string*)fieldAddress;
                        std::string fieldName = parentName + field.name;

                        std::string buffer;
                        buffer.reserve(INT8_MAX);
                        buffer = *stringAddress;

                        if (ImGui::InputText(fieldName.c_str(), buffer.data(), buffer.capacity()))
                        {
                            valueChanged |= *stringAddress != buffer;
                            *stringAddress = buffer;
                        }
                    }
                    break;

                case MirrorTypes::m_charPtr:
                case MirrorTypes::m_constCharPtr:
                    {
                        const char** constCharPtrAddress = (const char**)fieldAddress;
                        std::string fieldName = parentName + field.name;

                        std::string buffer;
                        buffer.reserve(INT8_MAX);
                        buffer = *constCharPtrAddress;

                        if (ImGui::InputText(fieldName.c_str(), buffer.data(), buffer.capacity()))
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
                        char* charPtrAddress = (char*)fieldAddress;
                        char charEscaped[2] = { *charPtrAddress, '\0' };
                        std::string fieldName = parentName + field.name;
                        if (ImGui::InputText(fieldName.c_str(), charEscaped, 2))
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
                        bool* boolAddress = (bool*)fieldAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::Checkbox(fieldName.c_str(), boolAddress))
                        {
                            valueChanged = true;
                        }
                    }
                    break;

                case MirrorTypes::m_float:
                    {
                        float* numberAddress = (float*)fieldAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragFloat(fieldName.c_str(), numberAddress, .1f))
                        {
                            valueChanged = true;
                        }
                    }
                    break;


                case MirrorTypes::m_eSceneTypes:
                case MirrorTypes::m_uint8_t:
                    {
                        u8* numberAddress = (u8*)fieldAddress;
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (u8)temp;
                            *numberAddress = (u8)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_uint16_t:
                    {
                        u16* numberAddress = (u16*)fieldAddress;
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (u16)temp;
                            *numberAddress = (u16)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_uint32_t:
                    {
                        uint32_t* numberAddress = (uint32_t*)fieldAddress;
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;

                        if (ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (uint32_t)temp;
                            *numberAddress = (uint32_t)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_uint64_t:
                    {
                        uint64_t* numberAddress = (uint64_t*)fieldAddress;
                        std::string fieldName = parentName + field.name;

                        std::string numberAsString = std::to_string(*numberAddress);
                        if (ImGui::InputText(fieldName.c_str(), numberAsString.data(), numberAsString.size(), ImGuiInputTextFlags_ReadOnly))
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
                        if (false && ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (uint64_t)temp;
                            *numberAddress = (uint64_t)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_int8_t:
                    {
                        s8* numberAddress = (s8*)fieldAddress;
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (s8)temp;
                            *numberAddress = (s8)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_int16_t:
                    {
                        s16* numberAddress = (s16*)fieldAddress;
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (s16)temp;
                            *numberAddress = (s16)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_int32_t:
                case MirrorTypes::m_int:
                    {
                        int* numberAddress = (int*)fieldAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::DragInt(fieldName.c_str(), numberAddress))
                        {
                            valueChanged = true;
                        }
                    }
                    break;

                case MirrorTypes::m_int64_t:
                    {
                        int64_t* numberAddress = (int64_t*)fieldAddress;
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;

                        if (ImGui::DragInt(fieldName.c_str(), &temp))
                        {
                            valueChanged |= *numberAddress != (int64_t)temp;
                            *numberAddress = (int64_t)temp;
                        }
                    }
                    break;

                case MirrorTypes::m_double:
                    {
                        double* numberAddress = (double*)fieldAddress;
                        float temp = (float)*numberAddress;
                        std::string fieldName = parentName + field.name;

                        if (ImGui::DragFloat(fieldName.c_str(), &temp)) // #TODO Review need for precision
                        {
                            valueChanged |= *numberAddress != (double)temp;
                            *numberAddress = (double)temp;
                        }
                    }
                    break;

                default:
                    // #TODO Review potential for infinite loop
                    if (field.typeInfo)
                    {
                        parentName += field.name + " ";
                        valueChanged |= InspectFieldRecursive(field.typeInfo, (char*)obj + field.offset, parentName);
                        parentName.clear();
                    }
                    else if (!hasWarned)
                    {
                        LOG_WARN("{0} Unsupported field type {1} {2}({3}) for inspection", __FUNCTION__, field.name, field.typeInfo->stringName, (int)field.typeInfo->enumType);
                        hasWarned = true;
                    }

                    break;
                }
            }
#endif
            return valueChanged;
        }

    }

}
