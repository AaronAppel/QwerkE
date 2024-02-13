#include "QE_ResourceViewer.h"

#include "Libraries/enum/QC_enum.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_Constants.h"
#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_FrameBufferObject.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Material.h"
#include "QF_Mesh.h"
#include "QF_Texture.h"
#include "QF_ShaderProgram.h"
#include "QF_GameObject.h"
#include "QF_RenderComponent.h"
#include "QF_CameraComponent.h"
#include "QF_Scene.h"
#include "QF_Serialization.h"
#include "QF_Scenes.h"
#include "QF_Resources.h"
#include "QF_Audio.h"
#include "QF_Factory.h"
#include "QF_Window.h"

#include "QE_MaterialEditor.h"

namespace QwerkE {

    ResourceViewer::ResourceViewer()
    {
        m_MaterialEditor = new MaterialEditor();
        m_Materials = Resources::SeeMaterials();
        m_Textures = Resources::SeeTextures();
        m_Shaders = Resources::SeeShaderPrograms();
        m_Meshes = Resources::SeeMeshes();
        m_Sounds = Resources::SeeSounds();

        m_ViewerScene = new Scene("ThumbNail.qscene");
        m_ViewerScene->LoadScene();
        ComponentCamera* camera = (ComponentCamera*)m_ViewerScene->GetCameraList().at(0)->GetComponent(Component_Camera);
        camera->SetViewportSize(Window::GetResolution());

        RenderModelThumbnails();
    }

    ResourceViewer::~ResourceViewer()
    {
        m_ModelImageHandles.clear();
        delete m_MaterialEditor;
    }

    QC_ENUM(eResourceViewerDrawTypes, int, Textures, Materials, Shaders, Font, Models, Sounds);

    const char* s_resourceViewerWindowTitle = "Resources";
    const char* s_refreshButtonText = "Refresh";

    void ResourceViewer::Draw()
    {
        if (!ImGui::Begin(s_resourceViewerWindowTitle)) { return; }

        const int buttonsPerRow = 3;
        for (size_t i = 0; i < eResourceViewerDrawTypes::_size_constant; i++)
        {
            if (i % buttonsPerRow != 0) ImGui::SameLine();

            if (ImGui::Button(ENUM_TO_STR(eResourceViewerDrawTypes::_from_index(i))))
            {
                m_CurrentResource = i;
            }
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - (strlen(s_refreshButtonText) * 9.f));
        if (ImGui::Button(s_refreshButtonText))
        {
            RenderModelThumbnails();
        }

        ImGui::Separator();

        ImVec2 winSize = ImGui::GetWindowSize();
        m_ModelsPerRow = (unsigned char)(winSize.x / (m_ModelThumbnailPixelSize.x * 1.5f) + 1.0f); // (* up the image size for feel), + avoid dividing by 0

        const ImVec2 tooltipOnClickSize = ImVec2(256, 256); // #TODO Consider moving to an editor settings file
        const ImVec2 imageUv0 = ImVec2(0.0f, 1.0f);
        const ImVec2 imageUv1 = ImVec2(1.0f, 0.0f);
        const int imageFramePadding = 1;

        unsigned int counter = 0;
        switch (m_CurrentResource) // #TODO Looks like there's only 2 cases needed if the arguments change. Maybe use textureId function or if statements
        {
        case eResourceViewerDrawTypes::Textures:
            for (const auto& p : *m_Textures)
            {
                if (counter % m_ModelsPerRow) ImGui::SameLine();

                ImGui::ImageButton((ImTextureID)p.second->s_Handle, m_ModelThumbnailPixelSize, imageUv0, imageUv1, imageFramePadding);

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();

                    if (ImGui::IsMouseDown(0))
                    {
                        ImGui::ImageButton((ImTextureID)p.second->s_Handle, tooltipOnClickSize, imageUv0, imageUv1, imageFramePadding);
                    }

                    ImGui::Text(p.second->s_FileName.c_str());
                    ImGui::Text(std::to_string(p.second->s_Handle).c_str());
                    ImGui::EndTooltip();
                }
                counter++;
            }
            break;

        case eResourceViewerDrawTypes::Materials:
            for (const auto& p : *m_Materials)
            {
                if (counter % m_ModelsPerRow) ImGui::SameLine();

                if (Texture* diffuseTexture = p.second->GetMaterialByType(eMaterialMaps::MatMap_Diffuse))
                {
                    ImTextureID textureId = (ImTextureID)diffuseTexture->s_Handle;
                    ImGui::ImageButton(textureId, m_ModelThumbnailPixelSize, imageUv0, imageUv1, imageFramePadding);
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        // #TODO Image name or something might be better. use newly create asset tags
                        ImGui::Text(p.second->GetMaterialName().c_str());
                        ImGui::Text(std::to_string(diffuseTexture->s_Handle).c_str());
                        ImGui::EndTooltip();
                    }
                    if (ImGui::IsItemClicked())
                    {
                        m_ShowMatEditor = true;
                        m_MatName = p.second->GetMaterialName();
                    }
                }
                else
                {
                    Texture* nullTexture = Resources::GetTexture(null_texture);
                    ImGui::ImageButton((ImTextureID)nullTexture->s_Handle, m_ModelThumbnailPixelSize, imageUv0, imageUv1, imageFramePadding);
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        // #TODO Image name or something might be better. use newly create asset tags
                        ImGui::Text(nullTexture->s_FileName.c_str());
                        ImGui::Text(std::to_string(nullTexture->s_Handle).c_str());
                        ImGui::EndTooltip();
                    }
                }
                counter++; // #TODO Review
            }
            break;

        case eResourceViewerDrawTypes::Shaders:
            for (auto p : *m_Shaders)
            {
                if (counter % m_ModelsPerRow) ImGui::SameLine();

                if (ImGui::Button(p.first.c_str()))
                {
                    // #TODO Open shader editor with selected shader open for editing
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text(std::to_string(p.second->GetProgram()).c_str());
                    ImGui::EndTooltip();
                }
                counter++;
            }
            break;

        case eResourceViewerDrawTypes::Font:
            break;

        case eResourceViewerDrawTypes::Models:
            for (unsigned int i = 0; i < m_ModelImageHandles.size(); i++)
            {
                if (counter % m_ModelsPerRow) ImGui::SameLine();

                ImGui::ImageButton((ImTextureID)m_ModelImageHandles.at(i), m_ModelThumbnailPixelSize, imageUv0, imageUv1, imageFramePadding);

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    if (ImGui::IsMouseDown(ImGui::Buttons::MouseLeft))
                    {
                        ImGui::ImageButton((ImTextureID)m_ModelImageHandles.at(i), tooltipOnClickSize, imageUv0, imageUv1, imageFramePadding);
                    }

                    ImGui::Text(std::to_string(m_ModelImageHandles[i]).c_str());
                    ImGui::EndTooltip();
                }
                counter++;
            }
            break;

        case eResourceViewerDrawTypes::Sounds:
            for (auto p : *m_Sounds)
            {
                if (counter % m_ModelsPerRow)
                    ImGui::SameLine();

                if (ImGui::Button(p.first.c_str()))
                {
                    Audio::PlaySound(p.first.c_str());
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text(std::to_string(p.second).c_str());
                    ImGui::EndTooltip();
                }
                counter++;
            }
            break;
        }

        if (m_ShowMatEditor)
        {
            m_MaterialEditor->Draw(Resources::GetMaterial(m_MatName.c_str()), &m_ShowMatEditor);
        }

        ImGui::End();
    }

    void ResourceViewer::RenderModelThumbnails()
    {
        ASSERT(m_ModelImageHandles.empty(), "Thumbnails have already been generated!");

        const std::map<std::string, GameObject*>& sceneObjects = m_ViewerScene->GetObjectList();

        for (auto p : sceneObjects)
        {
            p.second->Deactivate();
        }

        GameObject* currentCamera = m_ViewerScene->GetCurrentCamera();
        ComponentCamera* camComponent = (ComponentCamera*)currentCamera->GetComponent(eComponentTags::Component_Camera);
        camComponent->SetViewportSize(vec2(128, 128)); // #TODO Add an aspect ratio or some value to each individual camera

        for (auto p : sceneObjects)
        {
            p.second->Activate();

            FrameBufferObject fbo;
            fbo.Init();
            fbo.Bind();

            Renderer::ClearScreen();

            m_ViewerScene->Draw();

            m_ModelImageHandles.push_back(fbo.GetTextureID());

            fbo.UnBind();

            p.second->Deactivate();
        }
    }

}
