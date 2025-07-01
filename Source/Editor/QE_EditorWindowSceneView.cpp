#include "QE_EditorWindowSceneView.h"

#ifdef _QBGFX
#include "Libraries/bx/include/bx/rng.h" // #TODO Abstract out bgfx from QE domain
#endif

#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_ComponentCamera.h"
#include "QF_ComponentTransform.h"
#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_Paths.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialize.h"
#include "QF_Shader.h"

#include "QE_Settings.h"

namespace QwerkE {

    namespace Editor {


        static ImGuiStyle s_PlayModeStyle;
        static bool s_LoadedStyle = false; // #TODO Deprecate?

        EditorWindowSceneView::EditorWindowSceneView(GUID guid) :
            EditorWindow("Scene View", EditorWindowTypes::SceneView, guid)
        {
            Init();
        }

        void EditorWindowSceneView::Init()
        {
            m_ImGuiFlags = ImGuiWindowFlags_NoScrollbar;
            snprintf(m_ScenesComboLabelBuffer, sizeof(m_ScenesComboLabelBuffer), "Scenes: %i##%llu", 0, GetGuid());
            m_EditorCamera.m_ShowSphere = false;
            // #TODO Improve construction of matrices
            float temp[16] = {
                0.99999862909317,
                0,
                -0.0016666054725646973,
                0,
                2.7775738544733031e-06,
                0.99999862909317,
                0.0016666031442582607,
                0,
                0.0016666031442582607,
                -0.0016666054725646973,
                0.99999725818634033,
                0,
                -4.401646614074707,
                3.784186840057373,
                -6.67338514328003,
                1
            };
            memcpy(m_EditorCameraTransform.m_Matrix, temp, sizeof(float) * 16);

            // FBO 1
            // Create read texture
            const vec2f& windowSize = Window::GetSize();
            const bool has_mips = false;
            const uint16_t num_layers = 1;

            m_ViewId = Renderer::NextViewId();

            m_FrameBufferTextures[0].Init(windowSize.x, windowSize.y,
                has_mips, num_layers,
                bgfx::TextureFormat::BGRA8,
                BGFX_TEXTURE_RT);
            ASSERT(bgfx::isValid(m_FrameBufferTextures[0].TextureHandle()), "Error creating frame buffer texture [0]!");

            // Create write texture
            m_FrameBufferTextures[1].Init(windowSize.x, windowSize.y,
                has_mips, num_layers,
                bgfx::TextureFormat::D16,
                BGFX_TEXTURE_RT_WRITE_ONLY);
            ASSERT(bgfx::isValid(m_FrameBufferTextures[1].TextureHandle()), "Error creating frame buffer depth texture [1]!");

            // Create FBO
            m_FrameBuffer.Init(2, m_FrameBufferTextures);
            ASSERT(bgfx::kInvalidHandle != m_FrameBuffer.FrameBufferHandle().idx, "Error creating frame buffer!");
            m_FrameBuffer.SetupView(m_ViewId, std::string("FBO_") + m_WindowName
                , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
                , 0x303030ff
                , 1.0f
                , 0
            );

            m_CurrentScene = Scenes::GetScene(m_CurrentSceneGuid); // #TODO Serialize constsruct using guid member
            if (nullptr == m_CurrentScene && GUID::Invalid != m_CurrentSceneGuid)
            {
                m_CurrentScene = Assets::Get<Scene>(m_CurrentSceneGuid);
            }

            if (!s_LoadedStyle)
            {
                Serialize::FromFile(Paths::Style(Settings::GetStyleFileName2()).c_str(), s_PlayModeStyle);
                s_LoadedStyle = true;
            }

            PickingSetup();
        }

        void EditorWindowSceneView::PickingSetup()
        {
            bx::RngMwc mwc;  // Random number generator

            for (uint32_t ii = 0; ii < m_Count; ++ii)
            {
                // For the sake of this example, we'll give each mesh a random color,  so the debug output looks colorful.
                // In an actual app, you'd probably just want to count starting from 1
                uint32_t rr = mwc.gen() % 256;
                uint32_t gg = mwc.gen() % 256;
                uint32_t bb = mwc.gen() % 256;
                m_idsF[ii][0] = rr / 255.0f;
                m_idsF[ii][1] = gg / 255.0f;
                m_idsF[ii][2] = bb / 255.0f;
                m_idsF[ii][3] = 1.0f;
                m_idsU[ii] = rr + (gg << 8) + (bb << 16) + (255u << 24);
            }

            // Set up screen clears
            m_ViewIdShadingPass = Renderer::NextViewId();
            m_ViewIdIdPass = Renderer::NextViewId();
            m_ViewIdBlitPass = Renderer::NextViewId();

            bgfx::setViewClear(m_ViewIdShadingPass
                , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
                , 0x303030ff
                , 1.0f
                , 0
            );

            // ID buffer clears to black, which represents clicking on nothing (background)
            bgfx::setViewClear(m_ViewIdIdPass
                , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
                , 0x000000ff
                , 1.0f
                , 0
            );

            m_highlighted = UINT32_MAX;
            m_reading = 0;
            m_currFrame = UINT32_MAX;

            // Create uniforms
            u_tint = bgfx::createUniform("u_tint", bgfx::UniformType::Vec4); // Tint for when you click on items
            u_id = bgfx::createUniform("u_id", bgfx::UniformType::Vec4); // ID for drawing into ID buffer

            m_shadingProgram = Assets::Get<Shader>(1034563466966038478);    // ("vs_picking_shaded", "fs_picking_shaded"); // Blinn shading
            m_idProgram = Assets::Get<Shader>(934563662924383823);         // ("vs_picking_shaded", "fs_picking_id");     // Shader for drawing into ID buffer

            // Set up ID buffer, which has a color target and depth buffer
            m_pickingRT = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::RGBA8, 0
                | BGFX_TEXTURE_RT
                | BGFX_SAMPLER_MIN_POINT
                | BGFX_SAMPLER_MAG_POINT
                | BGFX_SAMPLER_MIP_POINT
                | BGFX_SAMPLER_U_CLAMP
                | BGFX_SAMPLER_V_CLAMP
            );
            m_pickingRTDepth = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::D32F, 0
                | BGFX_TEXTURE_RT
                | BGFX_SAMPLER_MIN_POINT
                | BGFX_SAMPLER_MAG_POINT
                | BGFX_SAMPLER_MIP_POINT
                | BGFX_SAMPLER_U_CLAMP
                | BGFX_SAMPLER_V_CLAMP
            );

            // CPU texture for blitting to and reading ID buffer so we can see what was clicked on.
            // Impossible to read directly from a render target, you *must* blit to a CPU texture
            // first. Algorithm Overview: Render on GPU -> Blit to CPU texture -> Read from CPU
            // texture.
            m_blitTex = bgfx::createTexture2D(ID_DIM, ID_DIM, false, 1, bgfx::TextureFormat::RGBA8, 0
                | BGFX_TEXTURE_BLIT_DST
                | BGFX_TEXTURE_READ_BACK
                | BGFX_SAMPLER_MIN_POINT
                | BGFX_SAMPLER_MAG_POINT
                | BGFX_SAMPLER_MIP_POINT
                | BGFX_SAMPLER_U_CLAMP
                | BGFX_SAMPLER_V_CLAMP
            );

            bgfx::TextureHandle rt[2] =
            {
                m_pickingRT,
                m_pickingRTDepth
            };
            m_pickingFB = bgfx::createFrameBuffer(BX_COUNTOF(rt), rt, true);
        }

        void EditorWindowSceneView::DrawInternal()
        {
            if (nullptr == m_CurrentScene)
            {
                m_CurrentScene = Scenes::GetScene(m_CurrentSceneGuid);
                if (!m_CurrentScene)
                {
                    m_CurrentSceneGuid = GUID::Invalid;
                    m_LastSceneIndex = Scenes::GetCurrentSceneIndex();
                    m_CurrentScene = Scenes::GetCurrentScene();
                }
                else
                {
                    m_LastSceneIndex = Scenes::GetSceneIndex(m_CurrentScene);
                }
            }

            const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
            if (scenes.empty())
            {
                ImGui::Text("No scene selected");
                return;
            }

            if (!m_CurrentScene)
            {
                ImGui::Text("Null scene selected");
                m_CurrentSceneGuid = GUID::Invalid;
                return;
            }

            bool sceneWasDirty = m_CurrentScene->IsDirty();
            if (sceneWasDirty)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
            }

            std::string saveButtonName = "S##";
            saveButtonName += std::to_string(GetGuid());
            if (ImGui::Button(saveButtonName.c_str())) m_CurrentScene->SaveScene();
            if (sceneWasDirty)
            {
                ImGui::PopStyleColor(3);
            }
            ImGui::SameLine();
            std::string reloadButtonName = "R##";
            ImGui::SameLineIfSpace(1.f * ImGui::g_pixelsPerCharacterButton);
            saveButtonName += std::to_string(GetGuid());
            if (ImGui::Button(reloadButtonName.c_str()))
            {
                Editor::OnEntitySelected(EntityHandle());
                m_CurrentScene->ReloadScene();
                Editor::OnSceneReloaded(m_CurrentScene);
            }

            ImGuiStyle previousStyle = ImGui::GetStyle();
            if (!m_CurrentScene->GetIsPaused())
            {
                // #TODO Create style setter helper somewhere better
                ImGuiStyle& style = ImGui::GetStyle();
                style = s_PlayModeStyle;
            }

            constexpr float itemWidth = 80.f;

            ImGui::PushItemWidth(itemWidth);
            ImGui::SameLineIfSpace(itemWidth);
            ImGui::Text(("vID " + std::to_string(m_ViewId)).c_str());
            ImGui::PopItemWidth();

            ImGui::PushItemWidth(itemWidth);
            ImGui::SameLineIfSpace(itemWidth);
            ImGui::Text(("tID " + std::to_string(m_FrameBufferTextures[0].TextureHandle().idx)).c_str());
            ImGui::PopItemWidth();

            std::vector<const char*> sceneNames;
            sceneNames.reserve(3);

            for (size_t i = 0; i < scenes.size(); i++)
            {
                sceneNames.push_back(scenes[i]->GetSceneName().c_str());
            }

            constexpr float scenesDropDownScalar = 1.25f;
            const u32 sceneFileNameWidth = (u32)strlen(sceneNames[m_LastSceneIndex]) * ImGui::g_pixelsPerCharacter * scenesDropDownScalar;

            constexpr u32 s_DropDownArrowSize = 20;
            ImGui::PushItemWidth((float)sceneFileNameWidth + (float)s_DropDownArrowSize);
            ImGui::SameLineIfSpace((float)sceneFileNameWidth + (float)s_DropDownArrowSize);

            snprintf(m_ScenesComboLabelBuffer, sizeof(m_ScenesComboLabelBuffer), "##Scenes: %i##%llu", (int)sceneNames.size(), GetGuid());

            if (ImGui::Combo(m_ScenesComboLabelBuffer, &m_LastSceneIndex, sceneNames.data(), (s32)scenes.size()))
            {
                m_CurrentScene = scenes[m_LastSceneIndex];
                m_CurrentSceneGuid = m_CurrentScene->GetGuid();
                // #NOTE Scene transition changes removes above lines for below
                // #TODO SetActive(true/false) ?
                // Scenes::SetCurrentScene(index);
            }
            ImGui::PopItemWidth();

            ImGui::PushItemWidth(11.f * ImGui::g_pixelsPerCharacter); // #TODO Improve hard coded scaling
            ImGui::SameLineIfSpace(11.f * ImGui::g_pixelsPerCharacter);
            // #TODO Game window (texture) scaling options
            int listIndex = 0;
            std::string scalingComboBoxUniqueId = "Scaling##";
            scalingComboBoxUniqueId += GetGuid();
            if (ImGui::Combo(scalingComboBoxUniqueId.c_str(), &listIndex,
                "#TODO\0"
                "Ratio\0"
                "Size\0"
                "Fit Window\0"
                "Pixel\0"
            ))
            {
                // #TODO: Make Menu Ratios -> [16:9, 3:4, etc...]
                // #TODO: Make Menu Size -> [1024×576, 1280×720, etc..., Custom]
            }
            ImGui::PopItemWidth();

            // #TODO Camera selection drop down
            std::vector<const char*> cameraEntityNames = { "EditorCam" };
            u16 currentItemNameLength = 0 == m_CurrentCameraComboIndex ? 9 : 0;

            auto viewCameras = m_CurrentScene->ViewComponents<ComponentCamera>();
            u16 i = 0;
            for (auto& entity : viewCameras)
            {
                EntityHandle handle(m_CurrentScene, entity);
                cameraEntityNames.emplace_back(handle.EntityName().c_str());

                if (i + 1 == m_CurrentCameraComboIndex)
                {
                    currentItemNameLength = handle.EntityName().size();
                }
                ++i;
            }

            std::string timeScaleButtonName = "T##";
            ImGui::SameLineIfSpace(1.f * ImGui::g_pixelsPerCharacterButton);
            timeScaleButtonName += std::to_string(GetGuid());
            if (ImGui::Button(timeScaleButtonName.c_str()))
            {
                ImGui::OpenPopup("TimeControls");
            }

            std::string togglePauseButtonName = m_CurrentScene->GetIsPaused() ? "<##" : "||##";
            ImGui::SameLineIfSpace(1.f * ImGui::g_pixelsPerCharacterButton);
            togglePauseButtonName += std::to_string(GetGuid());
            if (ImGui::Button(togglePauseButtonName.c_str()) || (Input::KeyPressed(e_P) && ImGui::IsWindowFocused()))
            {
                if (!m_CurrentScene->GetIsPaused())
                {
                    ImGuiStyle& style = ImGui::GetStyle();
                    style = previousStyle;
                }
                m_CurrentScene->ToggleIsPaused();
            }

            std::string renderingCheckboxName = "R##Rendering" + std::to_string(GetGuid());
            ImGui::SameLineIfSpace(1.f * ImGui::g_pixelsPerCharacterButton);
            ImGui::Checkbox(renderingCheckboxName.c_str(), &m_RenderingScene);

            const float camerasItemWidth = currentItemNameLength * ImGui::g_pixelsPerCharacter + 20.f; // #NOTE Slight increase for shorter names
            ImGui::PushItemWidth(camerasItemWidth + s_DropDownArrowSize);
            // ImGui::SameLine(ImGui::GetContentRegionAvail().x - camerasItemWidth - s_DropDownArrowSize - 10.f); // #NOTE Leave 10.f for window options ":"
            ImGui::SameLineIfSpace(camerasItemWidth - s_DropDownArrowSize - 10.f);
            std::string cameraComboBoxUniqueId = "##Camera:";
            cameraComboBoxUniqueId += GetGuid();
            if (ImGui::Combo(cameraComboBoxUniqueId.c_str(), &m_CurrentCameraComboIndex, cameraEntityNames.data(), (int)cameraEntityNames.size()))
            {
                if (!UsingEditorCamera())
                {
                    // #TODO Change camera. Can use name or entity GUID to find camera, for now
                    for (auto& entity : viewCameras)
                    {
                        EntityHandle handle(m_CurrentScene, entity);
                        if (handle.EntityName() == cameraEntityNames[m_CurrentCameraComboIndex])
                        {
                            Scenes::GetCurrentScene()->SetCurrentCameraEntity(handle);
                        }
                    }
                }
            }
            ImGui::PopItemWidth();

            // Gizmo drawing
            if (m_LastSelectEntity)
            {
                const bool drawSelectedEntityGizmo = UsingEditorCamera() || !m_LastSelectEntity.HasComponent<ComponentCamera>() ||
                    m_LastSelectEntity != m_LastSelectEntity.GetScene()->GetCurrentCameraEntity();

                if (drawSelectedEntityGizmo)
                {
                    ComponentTransform& transform = m_LastSelectEntity.GetComponent<ComponentTransform>();
                    const vec3f& position = transform.GetPosition();
#ifdef _QDEBUG
                    {   // Debug drawer call
                        uint64_t bgfxState = 0
                            | BGFX_STATE_DEPTH_TEST_MASK
                            | BGFX_STATE_WRITE_Z
                            | BGFX_STATE_CULL_CW
                            | BGFX_STATE_CULL_CCW
                            | BGFX_STATE_MSAA
                            ;
                        bgfx::setState(bgfxState);
                        DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer();
                        debugDrawer.begin(m_ViewId, true);

                        debugDrawer.drawOrb(position.x, position.y, position.z, 2.f, Axis::X);
                        debugDrawer.end();
                    }
#endif
                }
            }

            // Scene view specific draws
            Debug::DrawSphere(m_ViewId, vec3f(.0f, .0f, .0f), 0.1f);
            Debug::DrawGrid(m_ViewId, vec3f(.0f, .0f, .0f), 50);

            if (ImGui::IsItemClicked(ImGui::Buttons::MouseRight))
            {
                ImGui::OpenPopup("CameraPerspective");
            }

            if (ImGui::BeginPopup("CameraPerspective"))
            {
                if (ImGui::MenuItem("Perspective"))
                {
                    m_EditorCamera.m_Perspective = true;
                }
                if (ImGui::MenuItem("Orthographic"))
                {
                    m_EditorCamera.m_Perspective = false;
                }
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("TimeControls"))
            {
                static float timeScale = 1.f; // #TODO Add Time::TimeScale
                if (ImGui::SliderFloat("Time", &timeScale, 0.f, 10.f)) // Max = FLT_MAX / 2.f
                {
                    Time::SetTimeScale(timeScale);
                }
                if (ImGui::IsItemClicked(ImGui::MouseRight))
                {
                    timeScale = 1.f;
                    Time::SetTimeScale(timeScale);
                }
                ImGui::EndPopup();
            }

            // #NOTE Order dependencies with ImGui::IsItemFocused() calls in EditorCameraUpdate()
            ImGui::Image(ImTextureID(m_FrameBufferTextures[0].TextureHandle().idx), ImGui::GetContentRegionAvail() * 0.5f, ImVec2(0, 0), ImVec2(1, 1));

            if (Input::MousePressed(QKey::e_MouseRight) && ImGui::IsItemHovered())
            {
                m_MouseStartedDraggingOnImage = true;
            }
            else if (m_MouseStartedDraggingOnImage && !Input::MouseDown(QKey::e_MouseRight))
            {
                m_MouseStartedDraggingOnImage = false;
            }

            if (UsingEditorCamera())
            {
                EditorCameraUpdate();
                if (m_RenderingScene)
                {
                    m_EditorCamera.PreDrawSetup(m_ViewId, m_EditorCameraTransform.GetPosition());
                    m_CurrentScene->Draw(m_EditorCamera, m_EditorCameraTransform.GetPosition(), m_ViewId);
                }
            }
            else if (m_RenderingScene)
            {
                m_CurrentScene->Draw(m_ViewId);
            }

            if (!m_CurrentScene->GetIsPaused())
            {
                ImGuiStyle& style = ImGui::GetStyle();
                style = previousStyle;
            }

            PickingUpdate();
        }

        void EditorWindowSceneView::PickingUpdate()
        {
            const vec2f& windowSize = Window::GetSize();
            ImGui::Image(ImTextureID(m_pickingRT.idx), ImVec2(windowSize.x / 5.0f - 16.0f, windowSize.x / 5.0f - 16.0f));

            bgfx::setViewFrameBuffer(m_ViewIdIdPass, m_pickingFB);

            bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
            bx::Vec3 eye =
            {
                0.0f,
                0.0f,
                0.0f
            };

            EntityHandle cameraHandle = m_CurrentScene->GetCurrentCameraEntity();
            if (cameraHandle)
            {
                if (cameraHandle.HasComponent<ComponentCamera>())
                {
                    const ComponentCamera& cameraCamera = cameraHandle.GetComponent<ComponentCamera>();
                    at = {
                        cameraCamera.m_LookAtPosition.x,
                        cameraCamera.m_LookAtPosition.y,
                        cameraCamera.m_LookAtPosition.z
                    };
                }

                const ComponentTransform& cameraTransform = cameraHandle.GetComponent<ComponentTransform>();
                const vec3f pos = cameraTransform.GetPosition();
                eye = {
                    pos.x,
                    pos.y,
                    pos.z
                };
            }

            float view[16];
            bx::mtxLookAt(view, eye, at);

            static bool homogeneousDepth = false;
            float proj[16];
            bx::mtxProj(proj, 60.0f, float(windowSize.x) / float(windowSize.y), 0.1f, 100.0f, homogeneousDepth);

            // Set up view rect and transform for the shaded pass
            bgfx::setViewRect(m_ViewIdShadingPass, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));
            bgfx::setViewTransform(m_ViewIdShadingPass, view, proj);

            // Set up picking pass
            float viewProj[16];
            bx::mtxMul(viewProj, view, proj);

            float invViewProj[16];
            bx::mtxInverse(invViewProj, viewProj);

            // Mouse coord in NDC
            float mouseXNDC = (Input::MousePos().x / (float)windowSize.x) * 2.0f - 1.0f;
            float mouseYNDC = ((windowSize.y - Input::MousePos().y) / (float)windowSize.y) * 2.0f - 1.0f;

            const bx::Vec3 pickEye = bx::mulH({ mouseXNDC, mouseYNDC, 0.0f }, invViewProj);
            const bx::Vec3 pickAt = bx::mulH({ mouseXNDC, mouseYNDC, 1.0f }, invViewProj);

            // Look at our unprojected point
            float pickView[16];
            bx::mtxLookAt(pickView, pickEye, pickAt);

            // Tight FOV is best for picking
            float fov = 3.f;
            float pickProj[16];
            bx::mtxProj(pickProj, fov, 1, 0.1f, 100.0f, homogeneousDepth);

            // View rect and transforms for picking pass
            bgfx::setViewRect(m_ViewIdIdPass, 0, 0, ID_DIM, ID_DIM);
            bgfx::setViewTransform(m_ViewIdIdPass, pickView, pickProj);

            // Picking highlights a mesh so we'll set up this tint color
            const float tintBasic[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            const float tintHighlighted[4] = { 0.3f, 0.3f, 2.0f, 1.0f };

            // Submit mesh to both of our render passes
            // Set uniform based on if this is the highlighted mesh
            GUID m_DummyGuid = 0;
            GUID m_HighlightedGuid = 1;
            bgfx::setUniform(u_tint
                , m_DummyGuid == m_HighlightedGuid
                ? tintHighlighted
                : tintBasic
            );
            // m_CurrentScene->Draw(RENDER_PASS_SHADING); // m_shadingProgram

            // Submit ID pass based on mesh ID
            u8 meshIndex = 0;
            bgfx::setUniform(u_id, m_idsF[meshIndex]); // #TODO Change per mesh during rendering
            m_CurrentScene->Draw(m_ViewIdIdPass); // m_idProgram

            // If the user previously clicked, and we're done reading data from GPU, look at ID buffer on CPU
            // Whatever mesh has the most pixels in the ID buffer is the one the user clicked on.
            if (m_reading == m_currFrame)
            {
                m_reading = 0;
                std::map<uint32_t, uint32_t> ids;  // This contains all the IDs found in the buffer
                uint32_t maxAmount = 0;
                for (uint8_t* x = m_blitData; x < m_blitData + ID_DIM * ID_DIM * 4;)
                {
                    uint8_t rr = *x++;
                    uint8_t gg = *x++;
                    uint8_t bb = *x++;
                    uint8_t aa = *x++;

                    if (0 == (rr | gg | bb)) // Skip background
                    {
                        continue;
                    }

                    uint32_t hashKey = rr + (gg << 8) + (bb << 16) + (aa << 24);
                    std::map<uint32_t, uint32_t>::iterator mapIter = ids.find(hashKey);
                    uint32_t amount = 1;
                    if (mapIter != ids.end())
                    {
                        amount = mapIter->second + 1;
                    }

                    ids[hashKey] = amount; // Amount of times this ID (color) has been clicked on in buffer
                    maxAmount = maxAmount > amount
                        ? maxAmount
                        : amount
                        ;
                }

                uint32_t idKey = 0;
                m_highlighted = UINT32_MAX;
                if (maxAmount)
                {
                    for (std::map<uint32_t, uint32_t>::iterator mapIter = ids.begin(); mapIter != ids.end(); mapIter++)
                    {
                        if (mapIter->second == maxAmount)
                        {
                            idKey = mapIter->first;
                            break;
                        }
                    }

                    for (uint32_t ii = 0; ii < 12; ++ii)
                    {
                        if (m_idsU[ii] == idKey)
                        {
                            m_highlighted = ii;
                            break;
                        }
                    }
                }
            }

            // Start a new readback?
            if (!m_reading && Input::MousePressed(e_MouseLeft))
            {
                // Blit and read
                bgfx::blit(m_ViewIdBlitPass, m_blitTex, 0, 0, m_pickingRT);
                m_reading = bgfx::readTexture(m_blitTex, m_blitData);
            }
        }

        void EditorWindowSceneView::PickingShutdown()
        {
            // Cleanup.
            bgfx::destroy(u_tint);
            bgfx::destroy(u_id);

            bgfx::destroy(m_pickingFB);
            bgfx::destroy(m_pickingRT);
            bgfx::destroy(m_pickingRTDepth);
            bgfx::destroy(m_blitTex);

            // Shutdown bgfx.
            bgfx::shutdown();
        }

        void EditorWindowSceneView::EditorCameraUpdate()
        {
            // #NOTE Support editor camera zoom when scene view image is hovered, even if window is not focused
            if (ImGui::IsItemHovered())
            {
                const float mouseScroll = Input::MouseScrollDelta();
                if (mouseScroll != 0.f)
                {
                    m_EditorCamera.m_Fov -= mouseScroll;
                }
            }

            // #TODO Keyboard input on window focused, mouse only on item focus or dragging
            if (!ImGui::IsWindowFocused())
            {
                if (m_IsLastFocusedSceneView)
                {
                    // Gamepad only input?
                }
                return;
            }

            const float unscaledDeltaTime = Time::PreviousFrameDurationUnscaled();
            static float pixelRatio = 5.f; // #TODO Review name and purpose. Higher values mean slower camera movement

            // #TODO Abstract input type. Maybe need a context to only activate if item/window focused
            if (m_MouseStartedDraggingOnImage)
            {
                // Mouse (look)
                if (Input::MouseDown(QKey::e_MouseRight))
                {
                    static float yaw = 0.f;
                    yaw += Input::MouseDelta().x / pixelRatio * unscaledDeltaTime;

                    // Pitch transform.m_Matrix[6];
                    static float pitch = 0.f;
                    pitch += Input::MouseDelta().y / pixelRatio * unscaledDeltaTime;

                    constexpr bx::Vec3 scale = { 1.f, 1.f, 1.f };
                    bx::Vec3 rotate = { pitch, yaw, 0.f };
                    const vec3f& translate = m_EditorCameraTransform.GetPosition();

                    bx::mtxSRT(m_EditorCameraTransform.m_Matrix,
                        scale.x, scale.y, scale.z,
                        rotate.x, rotate.y, rotate.z,
                        translate.x, translate.y, translate.z);
                }
            }

            // Keyboard (movement)
            const vec3f transformForward = m_EditorCameraTransform.Forward(); // #TODO Calculate and use proper forward
            const bx::Vec3 forward =
            {
                transformForward.x,
                transformForward.y,
                transformForward.z
            };

            const Input::GameActions& gameActions = Input::GetGameActions();

            const float moveSpeedMultiplier = Input::KeyDown(QKey::e_ShiftAny) ? 8.f : 1.f; // #TODO Move hard coded value

            constexpr float controllerStickDeadzone = 0.07f;

            if (Input::KeyDown(gameActions.Camera_MoveForward) || Input::GamepadAxis(e_QGamepadAxisLeftStick).y < -controllerStickDeadzone)
            {
                // #TODO Use stick magnitude to allow different speed based on stick deviation
                vec3f pos = m_EditorCameraTransform.GetPosition();
                bx::Vec3 eye = bx::mad(forward, unscaledDeltaTime * m_EditorCamera.m_MoveSpeed * moveSpeedMultiplier, bx::Vec3(pos.x, pos.y, pos.z));
                m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
                // m_EditorCameraTransform.m_Matrix[14] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
            }
            if (Input::KeyDown(gameActions.Camera_MoveBackward) || Input::GamepadAxis(e_QGamepadAxisLeftStick).y > controllerStickDeadzone)
            {
                vec3f pos = m_EditorCameraTransform.GetPosition();
                bx::Vec3 eye = bx::mad(forward, unscaledDeltaTime * -m_EditorCamera.m_MoveSpeed * moveSpeedMultiplier, bx::Vec3(pos.x, pos.y, pos.z));
                m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
                // m_EditorCameraTransform.m_Matrix[14] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
            }

            const bx::Vec3 right =
            {
                m_EditorCameraTransform.m_Matrix[0],
                m_EditorCameraTransform.m_Matrix[4],
                m_EditorCameraTransform.m_Matrix[8]
            };

            if (Input::KeyDown(gameActions.Camera_MoveLeft) || Input::GamepadAxis(e_QGamepadAxisLeftStick).x < -controllerStickDeadzone)
            {
                vec3f pos = m_EditorCameraTransform.GetPosition();
                bx::Vec3 eye = bx::mad(right, -unscaledDeltaTime * m_EditorCamera.m_MoveSpeed * moveSpeedMultiplier, bx::Vec3(pos.x, pos.y, pos.z));
                m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
                // m_EditorCameraTransform.m_Matrix[12] -= (m_EditorCamera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
            }
            if (Input::KeyDown(gameActions.Camera_MoveRight) || Input::GamepadAxis(e_QGamepadAxisLeftStick).x > controllerStickDeadzone)
            {
                vec3f pos = m_EditorCameraTransform.GetPosition();
                bx::Vec3 eye = bx::mad(right, unscaledDeltaTime * m_EditorCamera.m_MoveSpeed * moveSpeedMultiplier, bx::Vec3(pos.x, pos.y, pos.z));
                m_EditorCameraTransform.SetPosition(vec3f(eye.x, eye.y, eye.z));
                // m_EditorCameraTransform.m_Matrix[12] += (m_EditorCamera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
            }

            constexpr u8 yPositionIndex = 13;
            if (Input::KeyDown(gameActions.Camera_MoveDown) || Input::GamepadDown(e_GamepadBumperLeft))
            {
                m_EditorCameraTransform.m_Matrix[yPositionIndex] -= (m_EditorCamera.m_MoveSpeed * moveSpeedMultiplier * unscaledDeltaTime);
            }
            if (Input::KeyDown(gameActions.Camera_MoveUp) || Input::GamepadDown(e_GamepadBumperRight))
            {
                m_EditorCameraTransform.m_Matrix[yPositionIndex] += (m_EditorCamera.m_MoveSpeed * moveSpeedMultiplier * unscaledDeltaTime);
            }

            if (Input::KeyDown(gameActions.Camera_RotateRight) || Input::GamepadAxis(e_QGamepadAxisRightStick).x > controllerStickDeadzone)
            {
                // #TODO Fix rotation also changing position
                constexpr float rotationSpeed = 1.f;
                static float angle = 0.f;
                // angle += rotationSpeed * deltaTime;
                float rotationMatrix[16];
                bx::mtxRotateXYZ(rotationMatrix, 0.f, rotationSpeed * unscaledDeltaTime, 0.f);
                bx::mtxMul(m_EditorCameraTransform.m_Matrix, m_EditorCameraTransform.m_Matrix, rotationMatrix);
                // LOG_TRACE("{0} Camera rotate right", __FUNCTION__);
            }
            if (Input::KeyDown(gameActions.Camera_RotateLeft) || Input::GamepadAxis(e_QGamepadAxisRightStick).x < -controllerStickDeadzone)
            {
                constexpr float rotationSpeed = 1.f;
                static float angle = 0.f;
                // angle += rotationSpeed * deltaTime;
                float rotationMatrix[16];
                bx::mtxRotateXYZ(rotationMatrix, 0.f, -rotationSpeed * unscaledDeltaTime, 0.f);
                bx::mtxMul(m_EditorCameraTransform.m_Matrix, m_EditorCameraTransform.m_Matrix, rotationMatrix);
                // LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
            }
            if (Input::GamepadAxis(e_QGamepadAxisRightStick).y > controllerStickDeadzone)
            {
                constexpr float rotationSpeed = 0.5f;
                float rotationMatrix[16];
                bx::mtxRotateXYZ(rotationMatrix, rotationSpeed * unscaledDeltaTime, 0.f, 0.f);
                bx::mtxMul(m_EditorCameraTransform.m_Matrix, m_EditorCameraTransform.m_Matrix, rotationMatrix);
                // LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
            }
            if (Input::GamepadAxis(e_QGamepadAxisRightStick).y < -controllerStickDeadzone)
            {
                constexpr float rotationSpeed = 0.5f;
                float rotationMatrix[16];
                bx::mtxRotateXYZ(rotationMatrix, -rotationSpeed * unscaledDeltaTime, 0.f, 0.f);
                bx::mtxMul(m_EditorCameraTransform.m_Matrix, m_EditorCameraTransform.m_Matrix, rotationMatrix);
                // LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
            }

            if (const bool useTargetLookAt = false)
            {
                // #TODO Add option to reference an existing transform component in the scene
                vec3f targetPosition = m_EditorCamera.m_LookAtPosition;
                m_EditorCamera.m_LookAtPosition = targetPosition;
            }
            else if (const bool useDirectionalLookAt = true)
            {
                // #TODO Only re-calculate if forward changed
                constexpr float scalar = 1.f;
                const float* position = nullptr;
                vec3f forwardPosition = m_EditorCameraTransform.GetPosition() + (m_EditorCameraTransform.Forward() * scalar);
                m_EditorCamera.m_LookAtPosition = forwardPosition;
            }
        }

    }

}