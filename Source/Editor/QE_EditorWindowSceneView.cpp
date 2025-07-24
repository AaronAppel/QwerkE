#include "QE_EditorWindowSceneView.h"

#ifdef _QBGFX
#include "Libraries/bx/include/bx/rng.h" // #TODO Abstract out bgfx from QE domain
#endif

#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_ComponentCamera.h"
#include "QF_ComponentPhysics.h"
#include "QF_ComponentTransform.h"
#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_Paths.h"
#include "QF_PhysicsWorld.h"
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

        EditorWindowSceneView::~EditorWindowSceneView()
        {
            bgfx::destroy(u_tint);
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

            u_tint = bgfx::createUniform("u_tint", bgfx::UniformType::Vec4); // Tint for when you click on items
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

                        vec3f scale = transform.GetScale();
                        float magnitude = Math::Magnitude(scale);

                        debugDrawer.drawOrb(position.x, position.y, position.z, 0.55 * magnitude, Axis::X);
                        debugDrawer.end();
                    }
#endif
                }
            }

            // Scene view specific draws
            Debug::DrawSphere(m_ViewId, vec3f(.0f, .0f, .0f), 0.1f);
            Debug::DrawGrid(m_ViewId, vec3f(.0f, .0f, .0f), 50);

            auto physicsComponents = m_CurrentScene->ViewComponents<ComponentPhysics>();
            for (auto& entity : physicsComponents)
            {
                EntityHandle handle(m_CurrentScene, entity);
                ComponentPhysics& physics = handle.GetComponent<ComponentPhysics>();
                ComponentTransform& transform = handle.GetComponent<ComponentTransform>();
                vec3f bodyPosition = physics.BodyPosition();
                transform.SetPosition(bodyPosition);

                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TODO Move physics debug drawing
                debugDrawer.begin(m_ViewId);
                switch (physics.Shape())
                {
                case Physics::BodyShapes::Sphere:
                    debugDrawer.drawSphere(bodyPosition.x, bodyPosition.y, bodyPosition.z, 0.5f);
                    break;
                case Physics::BodyShapes::Box:
                    // #TODO transform.Down()
                    // #TODO Draw shapes using Jolt body properties or better support all shapes
                    debugDrawer.drawQuad({ 0, -1, 0 }, { bodyPosition.x, bodyPosition.y, bodyPosition.z }, transform.GetScale().length());
                    break;
                }
                debugDrawer.end();

                if (Input::KeyPressed(QKey::e_L))
                {
                    if (!physics.IsActive())
                    {
                        physics.SetActive(true);
                    }
                    physics.SetLinearVelocity(vec3f(0.0f, 5.0f, 0.0f));
                }
            }

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
            ImGui::Image(ImTextureID(m_FrameBufferTextures[0].TextureHandle().idx), ImGui::GetContentRegionAvail(), ImVec2(0, 0), ImVec2(1, 1));

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

            const float tintBasic[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            bgfx::setUniform(u_tint, tintBasic);
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

            static float yaw = 0.f;
            static float pitch = 0.f;
            constexpr float rotationSpeed = 10.f; // #TODO Determine which value to use below

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