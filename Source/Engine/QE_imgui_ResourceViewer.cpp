#include "QE_ResourceViewer.h"

#include "Libraries/enum/QC_enum.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_Constants.h"
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


#define RTL_NUMBER_OF_V1(A) (sizeof(A)/sizeof((A)[0]))
#define sizeofchar(A) sizeof(A)-1

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

        // m_Subject = ((Factory*)QwerkE::Services::GetService(eEngineServices::Factory_Entity))->CreateTestModel(m_ViewerScene, vec3(0, -3.5, 15));
        // m_Subject->SetRotation(vec3(0,180,0));

        m_AssetTagPlane = Factory::CreatePlane(m_ViewerScene, vec3(2, -2, 10));
        m_AssetTagPlane->SetRotation(vec3(90, 0, 0));
        m_AssetTagPlane->SetScale(vec3(0.3f, 0.3f, 0.3f));

        // m_ViewerScene->AddObjectToScene(m_Subject);
        m_ViewerScene->AddObjectToScene(m_AssetTagPlane);

        m_ViewerScene->Initialize();
        m_ViewerScene->SetIsEnabled(true);
        ((ComponentCamera*)m_ViewerScene->GetCameraList().at(0)->GetComponent(Component_Camera))->SetViewportSize(vec2(1, 1));

        Scenes::AddScene(m_ViewerScene);

        DrawModelThumbnails();
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
            DrawModelThumbnails();
        }

        ImVec2 winSize = ImGui::GetWindowSize();
        m_ModelsPerRow = (unsigned char)(winSize.x / (m_ModelThumbnailPixelSize.x * 1.5f) + 1.0f); // (* up the image size for feel), + avoid dividing by 0

        ImGui::Separator();

        unsigned int counter = 0;
        switch (m_CurrentResource) // #TODO Looks like there's only 2 cases needed if the arguments change. Maybe use a function or if statements
        {
        case eResourceViewerDrawTypes::Textures:
            for (const auto& p : *m_Textures)
            {
                if (counter % m_ModelsPerRow) ImGui::SameLine();

                ImGui::ImageButton((ImTextureID)p.second->s_Handle, m_ModelThumbnailPixelSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();

                    if (ImGui::IsMouseDown(0))
                    {
                        ImGui::ImageButton((ImTextureID)p.second->s_Handle, ImVec2(256, 256), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
                    }

                    ImGui::Text(p.second->s_FileName.c_str());
                    ImGui::Text(std::to_string(p.second->s_Handle).c_str());
                    //ImGui::Text("TagName");
                    ImGui::EndTooltip();
                }
                counter++;
            }
            break;

        case eResourceViewerDrawTypes::Materials:
            for (const auto& p : *m_Materials)
            {
                if (counter % m_ModelsPerRow)
                    ImGui::SameLine();

                ImGui::ImageButton((ImTextureID)p.second->GetMaterialByType(eMaterialMaps::MatMap_Diffuse)->s_Handle, m_ModelThumbnailPixelSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
                {
                    // Pressed. #TODO Look at moving IsItemClicked() logic here
                }
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    // #TODO Image name or something might be better. use newly create asset tags
                    ImGui::Text(p.second->GetMaterialName().c_str());
                    ImGui::Text(std::to_string(p.second->GetMaterialByType(eMaterialMaps::MatMap_Diffuse)->s_Handle).c_str());
                    ImGui::EndTooltip();
                }
                if (ImGui::IsItemClicked())
                {
                    m_ShowMatEditor = true;
                    m_MatName = p.second->GetMaterialName();
                }
                counter++;
            }
            break;

        case eResourceViewerDrawTypes::Shaders:
            for (auto p : *m_Shaders)
            {
                if (counter % m_ModelsPerRow) ImGui::SameLine();

                if (ImGui::Button(p.first.c_str()))
                {
                    // #TODO Implement
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
                if (counter % m_ModelsPerRow)
                    ImGui::SameLine();

                ImGui::ImageButton((ImTextureID)m_ModelImageHandles.at(i), m_ModelThumbnailPixelSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    if (ImGui::IsMouseDown(ImGui::Buttons::MouseLeft))
                    {
                        ImGui::ImageButton((ImTextureID)m_ModelImageHandles.at(i), ImVec2(256, 256), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
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

    void ResourceViewer::DrawModelThumbnails()
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

            // draw scene
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
