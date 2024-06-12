#include "QE_EditorInspector.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Buffer.h"
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

#include "QE_Mirror.h"

namespace QwerkE {

    namespace Inspector {

        bool local_InspectPrimitive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        bool local_InspectClass(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        bool local_InspectCollection(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);
        bool local_InspectPair(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);

        // #TODO Consider adding a read only bool argument
        // #TODO Look to make parentName a const&
        bool InspectType(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            // #TODO Support drag/drop targets, context menus (copy/paste, save/load, etc), pop ups, etc
            // if (ImGui::BeginDragDropTarget())
            // {
            //     if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            //     {
            //         const wchar_t* path = (const wchar_t*)payload->Data;
            //         std::filesystem::path texturePath(path);
            //     }
            //     ImGui::EndDragDropTarget();
            // }

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

            case Mirror::TypeId<uint8_t>():
            // case MirrorTypes::m_eSceneTypes:
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

            case Mirror::TypeId<uint16_t>():
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

            case Mirror::TypeId<uint32_t>():
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

            case Mirror::TypeId<uint64_t>():
                {
                    uint64_t* numberAddress = (uint64_t*)obj;

                    // Edit entity name
                    Buffer buffer(INT8_MAX); // #TODO Could be re-used/persistent and updated on entity change
                    buffer.Fill('\0');
                    strcpy(buffer.As<char>(), std::to_string(*numberAddress).c_str());

                    ImGui::PushItemWidth(165);
                    if (ImGui::InputText(elementName.c_str(), buffer.As<char>(), buffer.SizeInBytes()))
                    {
                        // #TODO Crash when clearing (delete/backspace) text contents to be empty
                        *numberAddress = std::stoull(buffer.As<char>());
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
                            std::string numberAsString = buffer.As<char>();
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

            case Mirror::TypeId<int8_t>():
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

            case Mirror::TypeId<int16_t>():
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

            case Mirror::TypeId<int32_t>():
                if (ImGui::DragInt(elementName.c_str(), (int32_t*)obj)) { valueChanged = true; } break;
            case Mirror::TypeId<int64_t>():
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

            case Mirror::TypeId<double>():
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
                    LOG_WARN("{0} Unsupported field type {1} {2}({3}) for inspection", __FUNCTION__, elementName, typeInfo->stringName.c_str(), (int)typeInfo->id);
                    hasWarned = true;
                }
                break;
            }
#endif
            return valueChanged;
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

            default:
                for (size_t i = 0; i < typeInfo->fields.size(); i++)
                {
                    const Mirror::Field field = typeInfo->fields[i];
                    if (field.flags & FieldSerializationFlags::_HideInInspector)
                        continue;

                    if (field.flags & FieldSerializationFlags::_InspectorViewOnly)
                    {
                        // #TODO Show but prevent editing
                    }

                    std::string fieldName = parentName + field.name + "##";
                    void* fieldAddress = (char*)obj + field.offset;
                    valueChanged |= InspectType(field.typeInfo, fieldAddress, fieldName);
                }
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
                            const u8 start = (u8)eScriptTypes::Invalid + 1;
                            const u8 range = (u8)eScriptTypes::COUNT;
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
            switch (typeInfo->id)
            {
            default:
                break;
            }
            return valueChanged;
        }

    }

}
