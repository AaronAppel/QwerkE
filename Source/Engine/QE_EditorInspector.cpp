#include "QE_EditorInspector.h"

#include "Libraries/imgui/QC_imgui.h"

#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"

#include "QF_Component.h"
#include "QF_CameraComponent.h"
#include "QF_RenderComponent.h"

#include "QF_Routine.h"
#include "QF_RenderRoutine.h"
#include "QF_TransformRoutine.h"

namespace QwerkE {

    namespace Inspector {

        static bool hasWarned = false;
        void InspectFieldRecursive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName)
        {
            const std::vector<Mirror::Field>& fields = typeInfo->fields;

            for (size_t i = 0; i < fields.size(); i++)
            {
                const Mirror::Field field = fields[i];

                const void* fieldAddress = (void*)((char*)obj + field.offset);

                switch (field.typeInfo->enumType)
                {
                case MirrorTypes::m_vector_renderable:
                    {
                        std::vector<Renderable>* renderables = (std::vector<Renderable>*)fieldAddress;

                        if (const Mirror::TypeInfo* collectionTypeInfo = field.typeInfo->collectionTypeInfo)
                        {
                            for (size_t i = 0; i < renderables->size(); i++)
                            {
                                parentName += field.name + " ";
                                InspectFieldRecursive(field.typeInfo->collectionTypeInfo, (void*)(renderables->data() + i), parentName);
                                // InspectFieldRecursive(Mirror::InfoForType<Renderable>(), (Renderable*)renderables->data() + i, parentName);
                                parentName.clear();
                            }
                        }
                    }
                    break;

                // case MirrorTypes::Component:
                //     {
                //         Component* component = (Component*)fieldAddress;
                //
                //         switch (component->GetTag())
                //         {
                //         case eComponentTags::Component_Render:
                //             std::vector<Renderable>* renderables = (std::vector<Renderable>*)fieldAddress;
                //             for (size_t i = 0; i < renderables->size(); i++)
                //             {
                //                 parentName += field.name;
                //                 InspectFieldRecursive(Mirror::InfoForType<Renderable>(), (Renderable*)renderables->data() + i, parentName);
                //                 parentName.clear();
                //             }
                //             break;
                //
                //         default:
                //             break;
                //         }
                //     }
                //     break;

                case MirrorTypes::m_vector_string:
                    {
                        std::vector<std::string>* strings = (std::vector<std::string>*)fieldAddress;
                        for (size_t i = 0; i < strings->size(); i++)
                        {
                            const std::string* stringAddress = (const std::string*)strings->data() + i;
                            std::string fieldName = parentName + field.name + std::to_string(i);
                            ImGui::LabelText(fieldName.c_str(), stringAddress->data());
                            // if ()
                            // {
                            //     Scenes::SetCurrentSceneDirty();
                            // }
                        }
                        break;
                    }
                    break;

                case MirrorTypes::m_vector_routinePtr:
                    {
                        std::vector<Routine*>* routines = (std::vector<Routine*>*)fieldAddress;

                        for (size_t i = 0; i < routines->size(); i++)
                        {
                            // #TODO Handle derived types
                            switch (routines->at(i)->GetRoutineType())
                            {
                            case QwerkE::Routine_Render:
                                InspectFieldRecursive(Mirror::InfoForType<RenderRoutine>(), routines->at(i), parentName);
                                break;

                            case QwerkE::Routine_Transform:
                                InspectFieldRecursive(Mirror::InfoForType<TransformRoutine>(), routines->at(i), parentName);
                                break;

                            case QwerkE::Routine_Physics:
                            case QwerkE::Routine_Print:
                            case QwerkE::Routine_Null:
                            case QwerkE::Routine_Max:
                            default:
                                LOG_WARN("{0} Unsupported field type {1} {2}({3}) for inspection", __FUNCTION__, field.name, field.typeInfo->stringName, (int)field.typeInfo->enumType);
                                break;
                            }
                        }
                        break;
                    }
                    break;

                case MirrorTypes::m_map_eComponentTags_componentPtr:
                {
                    std::map<eComponentTags, Component*>& components = *(std::map<eComponentTags, Component*>*)fieldAddress;
                    for (auto& it : components)
                    {
                        Component* component = it.second;

                        parentName += field.name + " ";
                        switch (component->GetTag()) // #TODO Look to handle sub classes more elegantly
                        {
                        case Component_Render:
                            InspectFieldRecursive(Mirror::InfoForType<RenderComponent>(), (RenderComponent*)component, parentName);
                            break;

                        case Component_Camera:
                            InspectFieldRecursive(Mirror::InfoForType<ComponentCamera>(), (ComponentCamera*)component, parentName);
                            break;

                        case Component_Light:
                        case Component_Physics:
                        case Component_Controller:
                        case Component_SkyBox:
                            // #TODO Implement component add
                            break;
                        }
                        parentName.clear();
                    }
                }
                break;

                case MirrorTypes::Vector2:
                {
                    float* vector2Address = (float*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    if (ImGui::DragFloat2(fieldName.c_str(), vector2Address, .1f))
                    {
                        Scenes::SetCurrentSceneDirty();
                    }
                }
                break;

                case MirrorTypes::Vector3:
                {
                    float* vector3Address = (float*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    if (ImGui::DragFloat3(fieldName.c_str(), vector3Address, .1f))
                    {
                        Scenes::SetCurrentSceneDirty();
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

                    if (ImGui::InputText(fieldName.c_str(), (char*)buffer.data(), buffer.capacity()))
                    {
                        *stringAddress = buffer;
                        Scenes::SetCurrentSceneDirty();
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

                    if (ImGui::InputText(fieldName.c_str(), (char*)buffer.data(), buffer.capacity()))
                    {
                        // delete *constCharPtrAddress;
                        *constCharPtrAddress = strdup(buffer.data());
                        Scenes::SetCurrentSceneDirty();
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
                        *charPtrAddress = charEscaped[0];
                        Scenes::SetCurrentSceneDirty();
                    }
                }
                break;

                case MirrorTypes::m_bool:
                {
                    bool* boolAddress = (bool*)fieldAddress;
                    int intValue = *boolAddress;
                    std::string fieldName = parentName + field.name;
                    if (ImGui::InputInt(fieldName.c_str(), &intValue))
                    {
                        *boolAddress = (bool)intValue;
                        Scenes::SetCurrentSceneDirty();
                    }
                }
                break;

                case MirrorTypes::m_float:
                {
                    float* numberAddress = (float*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    if (ImGui::DragFloat(fieldName.c_str(), numberAddress, .1f))
                    {
                        Scenes::SetCurrentSceneDirty();
                    }
                }
                break;

                case MirrorTypes::m_int8_t:
                case MirrorTypes::m_int16_t:
                case MirrorTypes::m_int32_t:
                case MirrorTypes::m_uint8_t:
                case MirrorTypes::m_uint16_t:
                case MirrorTypes::m_uint32_t:
                case MirrorTypes::m_int:
                case MirrorTypes::m_eSceneTypes:
                case MirrorTypes::m_eGameObjectTags:
                {
                    int* numberAddress = (int*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    if (ImGui::DragInt(fieldName.c_str(), numberAddress))
                    {
                        Scenes::SetCurrentSceneDirty();
                    }
                }
                break;

                case MirrorTypes::m_int64_t:
                case MirrorTypes::m_uint64_t:
                {
                    double* numberAddress = (double*)fieldAddress; // #TODO Implement
                }
                break;

                case MirrorTypes::m_double:
                {
                    double* numberAddress = (double*)fieldAddress; // #TODO Implement
                }
                break;

                default:
                    // #TODO Review potential for infinite loop
                    if (field.typeInfo)
                    {
                        int bp = 0;
                        parentName += field.name + " ";
                        InspectFieldRecursive(field.typeInfo, (char*)obj + field.offset, parentName);
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
        }

    }

}
