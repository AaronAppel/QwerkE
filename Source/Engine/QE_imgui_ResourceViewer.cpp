#include "QE_ResourceViewer.h"

#include "Libraries/enum/QC_enum.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_Constants.h"
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
#include "QF_ViewerScene.h"
#include "QF_Scenes.h"
#include "QF_Resources.h"
#include "QF_Audio.h"
#include "QF_Factory.h"

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

        m_FBO = new FrameBufferObject();
        m_FBO->Init();

        m_ViewerScene = new ViewerScene();

        m_AssetTagPlane = Factory::CreatePlane(m_ViewerScene, vec3(2, -2, 10));
        m_AssetTagPlane->SetRotation(vec3(90, 0, 0));
        m_AssetTagPlane->SetScale(vec3(0.3f, 0.3f, 0.3f));
        m_ViewerScene->AddObjectToScene(m_AssetTagPlane);

        m_ViewerScene->Initialize();
        m_ViewerScene->SetIsEnabled(true);
        ((ComponentCamera*)m_ViewerScene->GetCameraList().at(0)->GetComponent(Component_Camera))->SetViewportSize(vec2(1, 1));

        Scenes::AddScene(m_ViewerScene);

        RenderModelThumbnails();
    }

    ResourceViewer::~ResourceViewer()
    {
        delete m_MaterialEditor;
        delete m_FBO;
    }

    QC_ENUM(eResourceViewerDrawTypes, int, Textures, Materials, Shaders, Font, Models, Sounds);

    const char* s_resourceViewerWindowTitle = "Resources";
    const char* s_refreshButtonText = "Refresh";

    void ResourceViewer::Draw()
    {
        if (!ImGui::Begin(s_resourceViewerWindowTitle)) { return; }

        for (size_t i = 0; i < eResourceViewerDrawTypes::_size_constant; i++)
        {
            if (i > 0) ImGui::SameLine();

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

        const ImVec2 tooltipOnClickSize = ImVec2(256, 256);
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

                    ImGui::Text(std::to_string(m_ModelImageHandles[0]).c_str());
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
        // Dump old values. maybe calculate what changed in the future
        m_ModelImageHandles.clear();

        for (const auto& p : *m_Meshes)
        {
            m_FBO->Bind();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // #TODO Loop through renderables to setup
            //((RenderComponent*)m_Subject->GetComponent(Component_Render))->SetMeshAtIndex(0, m_Resources->GetMesh(null_mesh));

            // #TODO RenderRoutine needs to update its uniform functions properly
            //((RenderComponent*)m_Subject->GetComponent(Component_Render))->SetModel(p.second);
            //((RenderComponent*)m_AssetTagPlane->GetComponent(Component_Render))->SetColour(vec4(128, 128, 128, 255)); // #TODO use model asset tag color

            m_ViewerScene->Draw();

            // GLuint tempTexture;
            // glGenTextures(1, &tempTexture);
            // glBindTexture(GL_TEXTURE_2D, tempTexture);
            // glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, 0, 1);
            // glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1280, 720);

            m_FBO->UnBind();

            // m_ModelImageHandles.push_back(tempTexture);
        }
        m_ModelImageHandles.push_back(m_FBO->GetTextureID());
    }

}
