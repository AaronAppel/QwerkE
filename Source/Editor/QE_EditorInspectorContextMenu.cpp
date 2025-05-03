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

        static void local_ContextMenuCopy(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName);
        static bool local_ContextMenuPaste(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName);
        static bool local_ContextMenuAdditional(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName);

        bool priv_ContextMenu(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName)
        {
            // #NOTE React to imgui UI object interactions in local_InspectPrimitive()

            bool valueChanged = false;
            std::string popUpName = "Context ##EditorInspector";

            popUpName += elementName;
            if (ImGui::IsItemClicked(ImGui::MouseRight))
            {
                ImGui::OpenPopup(popUpName.c_str());
            }

            if (ImGui::BeginPopup(popUpName.c_str()))
            {
                local_ContextMenuCopy(typeInfo, obj, elementName);
                valueChanged |= local_ContextMenuPaste(typeInfo, obj, elementName);
                valueChanged |= local_ContextMenuAdditional(typeInfo, obj, elementName);

                // #TODO Look into BeginPopupContextWindow and OpenPopupOnItemClick
                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::BeginPopupContextItem("Test Context Item"))
                    {
                        ImGui::Button("Button");
                    }
                    ImGui::End();
                }
                // if (ImGui::OpenPopupOnItemClick)

                ImGui::EndPopup();
            }

            return valueChanged;
        }

        void local_ContextMenuCopy(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName)
        {
            if (ImGui::Button(("Copy To Clipboard##" + elementName).c_str()))
            {
                switch (typeInfo->id)
                {
                case Mirror::TypeId<u64>():
                    {
                        char buffer[U64_MAX_DIGITS + 1] = { '\0' };
                        u64* valueAddress = (u64*)obj;
                        strcpy(buffer, std::to_string(*valueAddress).c_str());

                        std::string numberAsString = buffer;
                        ImGui::SetClipboardText(numberAsString.c_str());
                        LOG_INFO("{0} Copied to clipboard: {1}", __FUNCTION__, numberAsString.c_str());
                        ImGui::CloseCurrentPopup();
                    }
                    break;
                default:
                    break;
                }
            }
        }

        bool local_ContextMenuPaste(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName)
        {
            bool valueChanged = false;

            if (ImGui::Button(("Paste From Clipboard##" + elementName).c_str()))
            {
                switch (typeInfo->id)
                {
                case Mirror::TypeId<u64>():
                {
                    std::string clipBoardText = ImGui::GetClipboardText();
                    LOG_INFO("{0} Pasting from clipboard: {1}", __FUNCTION__, clipBoardText.c_str());
                    u64* valueAddress = (u64*)obj;
                    *valueAddress = std::stoull(clipBoardText.c_str());
                    valueChanged = true;
                }
                break;
                default:
                    break;
                }
            }

            if (valueChanged)
            {
                ImGui::CloseCurrentPopup();
            }

            return valueChanged;
        }

        bool local_ContextMenuAdditional(const Mirror::TypeInfo* typeInfo, void* obj, const std::string& elementName)
        {
            bool valueChanged = false;

            // Additional type specific context menu options

            switch (typeInfo->id)
            {
            case Mirror::TypeId<u64>():
            case Mirror::TypeId<GUID>():
                if (ImGui::Button(("Regenerate GUID##" + elementName).c_str()))
                {
                    // #TODO Verify u64 is of type GUID
                    u64* valueAddress = (u64*)obj;
                    *valueAddress = GUID();
                    valueChanged = true;
                    LOG_INFO("{0} Regenerated GUID: {1}", __FUNCTION__, *valueAddress);
                }
                break;
            default:
                break;
            }

            if (valueChanged)
            {
                ImGui::CloseCurrentPopup();
            }

            return valueChanged;
        }

    }

}
