#include "QE_EditorInspector.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"

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

                const void* fieldAddress = (void*)((char*)obj + field.offset);

                switch (field.typeInfo->enumType)
                {
                case MirrorTypes::m_floatArray16:
                    {
                        float* f16matrix = (float*)fieldAddress;
                        ImGui::DragFloat4("0", f16matrix, .1f);
                        ImGui::DragFloat4("1", &f16matrix[4], .1f);
                        ImGui::DragFloat4("2", &f16matrix[8], .1f);
                        ImGui::DragFloat4("3", &f16matrix[12], .1f);
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
                            //     valueChanged = true;
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

                        if (ImGui::InputText(fieldName.c_str(), (char*)buffer.data(), buffer.capacity()))
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

                        if (ImGui::InputText(fieldName.c_str(), (char*)buffer.data(), buffer.capacity()))
                        {
                            // #TODO Review memory leak. delete *constCharPtrAddress;
                            char* result = strdup(buffer.data());
                            valueChanged |= strcmp(*constCharPtrAddress, result) != 0;
                            *constCharPtrAddress = result;
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
                        int intValue = *boolAddress;
                        std::string fieldName = parentName + field.name;
                        if (ImGui::InputInt(fieldName.c_str(), &intValue))
                        {
                            // #TODO Fix rounding where subtracting 0 gives 1, but adding 1 doesn't change the value. Avoid rounding from 0-1 = 1.
                            valueChanged |= *boolAddress != (bool)intValue;
                            *boolAddress = (bool)intValue;
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
                        int temp = (int)*numberAddress;
                        std::string fieldName = parentName + field.name;

                        if (ImGui::DragInt(fieldName.c_str(), &temp))
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
