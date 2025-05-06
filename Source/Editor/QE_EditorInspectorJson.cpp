#include "QE_EditorInspector.h"

#ifdef _QCJSON
#include "Libraries/cJSON/cJSON.h"
#endif

#include "QF_Buffer.h"
#include "QF_Files.h"

namespace QwerkE {

    namespace Inspector {

        void InspectJsonFileRaw(cJSON* jsonStructure)
        {
            if (!jsonStructure)
            {
                ImGui::Text("Invalid json structure");
                return;
            }

            std::stack<cJSON*> depthStack;
            depthStack.push(jsonStructure);

            cJSON* head = jsonStructure->child;

            const float indentationScaler = 10.f;

            ImGui::Text("{");
            ImGui::Indent(indentationScaler * 2); // #TODO avoid double 1st indent (avoid last '}' off left of window)

            while (!depthStack.empty())
            {
                if (!head)
                {
                    if (!depthStack.empty())
                    {
                        head = depthStack.top();
                        depthStack.pop();

                        ImGui::Unindent(indentationScaler);
                        if (head && head->next)
                        {
                            ImGui::Text("},"); // #TODO Still missing ',' in most places
                        }
                        else
                        {
                            ImGui::Text("}");
                        }
                    }
                    continue;
                }

                switch (head->type)
                {
                case 1 << 0: // cJSON_False
                {
                    std::string customFormat = "\"";
                    customFormat += head->string;
                    customFormat += "\": \"false\"";
                    ImGui::Text(customFormat.c_str());
                }
                break;
                case 1 << 1: // cJSON_True
                {
                    std::string customFormat = "\"";
                    customFormat += head->string;
                    customFormat += "\": \"true\"";
                    ImGui::Text(customFormat.c_str());
                }
                break;
                case 1 << 2: // cJSON_NULL
                {
                    std::string customFormat = "\"";
                    customFormat += head->string;
                    customFormat += "\": \"null\"";
                    ImGui::Text(customFormat.c_str());
                }
                break;
                case 1 << 3: // cJSON_Number
                {
                    if (head->valuedouble) // #TODO Int is also written with double so no way to know which to use
                    {
                        // #TODO Find out why %d loses precision. Maybe not supported by imgui formatting?
                        ImGui::Text("\"%s\": \"%f\"", head->string, head->valuedouble);
                    }
                    else
                    {
                        ImGui::Text("\"%s\": \"%i\"", head->string, head->valueint);
                    }
                }
                break;
                case 1 << 4: // cJSON_String
                {
                    std::string customFormat = "\"";
                    customFormat += head->string;
                    customFormat += "\": \"";
                    customFormat += head->valuestring;
                    customFormat += "\"";
                    ImGui::Text(customFormat.c_str());
                }
                break;
                case 1 << 5: // cJSON_Array
                {
                    std::string customFormat = "\"";
                    customFormat += head->string;
                    customFormat += "\": \"array\"";
                    ImGui::Text(customFormat.c_str());

                    // push head and continue?
                }
                break;
                case 1 << 6: // cJSON_Object
                {
                    std::string customFormat = "\"";
                    customFormat += head->string;
                    customFormat += "\": {";
                    ImGui::Text(customFormat.c_str());

                    // push head and continue?
                }
                break;
                case 1 << 7: // cJSON_Raw
                    break;
                    // case 0: // cJSON_Invalid (0)
                    // cJSON_IsReference 256
                    // cJSON_StringIsConst 512
                }

                if (head->child)
                {
                    ImGui::Indent(indentationScaler);
                    depthStack.push(head->next);
                    head = head->child;
                    continue;
                }

                head = head->next;
                if (head)
                {
                    ImGui::SameLine();
                    ImGui::Text(","); // #TODO Missing ',' on line 86 from GameScene1.qscene
                }
            }

            ImGui::Unindent(indentationScaler);
            ImGui::Text("}");
        }

        void InspectJsonFileWidgets(cJSON* jsonStructure)
        {

            // #TODO imgui tree nodes per descend
            // #TODO Write ImGui to represent json data
            // #TODO Show json file structure with widgets (buttons and w/e else) that can edit json structure
        }

        void InspectJsonFile(cJSON* jsonStructure, bool raw)
        {
            if (raw) // #TODO Deprecate if function is just a bool switch
            {
                InspectJsonFileRaw(jsonStructure);
            }
            else
            {
                InspectJsonFileWidgets(jsonStructure);
            }
        }

    }

}
