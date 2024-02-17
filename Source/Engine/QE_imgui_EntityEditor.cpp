#include "QE_EntityEditor.h"

#include "Libraries/imgui/QC_imgui.h"
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Bullet3/BulletDynamics/Dynamics/btRigidBody.h"

#include "QF_b3_PhysicsFactory.h" // #TODO Remove Bullet3 dependency from here
#include "QF_Bullet3Component.h"
#include "QF_Bullet3Routine.h"
#include "QF_Defines.h"
#include "QF_GameObject.h"
#include "QF_Log.h"
#include "QF_Mesh.h"
#include "QF_Physics.h"
#include "QF_RenderComponent.h"
#include "QF_Resources.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_Renderable.h"
#include "QF_RenderRoutine.h"

#include "QE_EditComponent.h"
#include "QE_Editor.h"

namespace QwerkE {

	EntityEditor::EntityEditor()
	{
        m_EditComponent = new EditComponent();

        if (Settings::GetEngineSettings().audioEnabled)
        {
            m_ListboxItemStrings.push_back("Sound Listener");
            m_ListboxItemStrings.push_back("Sound Player");
            m_ListboxItemTypes.push_back(eComponentTags::Component_SoundListener);
            m_ListboxItemTypes.push_back(eComponentTags::Component_SoundPlayer);
        }

        if (Settings::GetEngineSettings().physicsEnabled)
        {
            m_ListboxItemStrings.push_back("Physics");
            m_ListboxItemTypes.push_back(eComponentTags::Component_Physics);
        }
	}

	EntityEditor::~EntityEditor()
	{
        delete m_EditComponent;
	}

	void EntityEditor::Draw()
	{
        GameObject* currentEntity = nullptr;
        if (m_CurrentEntity)
        {
            currentEntity = m_CurrentEntity;
        }
        else if (Scene* currentScene = Scenes::GetCurrentScene())
        {
            const std::map<std::string, GameObject*>& list = currentScene->GetObjectList();
            auto begin = list.begin(); // #TODO Review logic
            if (begin != list.end())
            {
                m_CurrentEntity = begin->second;
            }
        }

		if (currentEntity != nullptr)
        {
            DrawEntityEditor();
            DrawEntityEditorInspect();
		}
	}

	void EntityEditor::DrawEntityEditor()
	{
        Scene* currentScene = Scenes::GetCurrentScene();
        if (!currentScene)
            return;

        if (ImGui::Begin("Entity Editor"))
        {
            //// Begin drawing entity data...
            // Draw generic GameObject data like transform and name
            // std::string name = m_CurrentEntity->GetName().c_str() + ' '; // extra space for editing
            if (ImGui::InputText("Name: ", (char*)m_CurrentEntity->GetName().c_str(), m_CurrentEntity->GetName().size()))
            {

            }
            // m_CurrentEntity->SetName(); // #TODO Scene map probably needs to handle name changes

            static float pos[3] = { 0.0f, 0.0f, 0.0f };
            pos[0] = m_CurrentEntity->GetPosition().x;
            pos[1] = m_CurrentEntity->GetPosition().y;
            pos[2] = m_CurrentEntity->GetPosition().z;

            static float rot[3] = { 0.0f, 0.0f, 0.0f };
            rot[0] = m_CurrentEntity->GetRotation().x;
            rot[1] = m_CurrentEntity->GetRotation().y;
            rot[2] = m_CurrentEntity->GetRotation().z;

            static float scale[3] = { 1.0f, 1.0f, 1.0f };
            scale[0] = m_CurrentEntity->GetScale().x;
            scale[1] = m_CurrentEntity->GetScale().y;
            scale[2] = m_CurrentEntity->GetScale().z;

            if (ImGui::InputFloat3("Pos: ", pos))
            {
                m_CurrentEntity->SetPosition(vec3(pos[0], pos[1], pos[2]));
            }
            if (ImGui::InputFloat3("Rot: ", rot))
            {
                m_CurrentEntity->SetRotation(vec3(rot[0], rot[1], rot[2]));
            }
            if (ImGui::InputFloat3("Scale: ", scale))
            {
                m_CurrentEntity->SetScale(vec3(scale[0], scale[1], scale[2]));
            }

            ImGui::Separator();

            static bool showComponentSelector = false;
            const char* enabledState = m_CurrentEntity->Enabled() ? "Deactivate" : "Activate";
            if (ImGui::Button(enabledState))
            {
                if (m_CurrentEntity->Enabled())
                {
                    m_CurrentEntity->Deactivate();
                }
                else
                {
                    m_CurrentEntity->Activate();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("+Component"))
            {
                showComponentSelector = true;
            }

            if (showComponentSelector)
            {
                static int listbox_item_current = eComponentTags::Component_Null;
                const int itemsHeight = 4;

                if (ImGui::ListBox("+Component", &listbox_item_current, m_ListboxItemStrings.data(), m_ListboxItemStrings.size(), itemsHeight))
                {
                    switch ((int)m_ListboxItemTypes.at(listbox_item_current))
                    {
                    case eComponentTags::Component_SoundListener:
                        if (!Settings::GetEngineSettings().audioEnabled)
                            break;
                        break;

                    case eComponentTags::Component_SoundPlayer:
                        if (!Settings::GetEngineSettings().audioEnabled)
                            break;
                        break;

                    case eComponentTags::Component_Render:
                        if (m_CurrentEntity->GetComponent(Component_Render) == nullptr)
                        {
                            Renderable renderable;
                            renderable.SetMaterial(Resources::GetMaterial(null_material));
                            renderable.SetShader(Resources::GetShaderProgram(null_shader));
                            renderable.SetMesh(Resources::GetMesh(null_mesh));
                            static int uniqueId = 0; // #FEATURE F0004, F0003
                            renderable.SetRenderableName(std::to_string(uniqueId++));

                            RenderComponent* rComp = (RenderComponent*)m_CurrentEntity->GetComponent(Component_Render);

                            if (rComp == nullptr)
                            {
                                rComp = new RenderComponent();
                                m_CurrentEntity->AddComponent(rComp);

                                if (m_CurrentEntity->GetFirstDrawRoutineOfType(Routine_Render) == nullptr)
                                {
                                    m_CurrentEntity->AddDrawRoutine((Routine*) new RenderRoutine());
                                }

                                rComp->AddRenderable(renderable);
                                rComp->GetParent()->GetFirstDrawRoutineOfType(Routine_Render)->Initialize();
                            }
                        }
                        break;

                    case eComponentTags::Component_Physics:
                        if (Settings::GetEngineSettings().physicsEnabled && m_CurrentEntity->GetComponent(Component_Physics) == nullptr)
                        {
                            if (!Settings::GetEngineSettings().physicsEnabled)
                                break;

                            vec3 position = m_CurrentEntity->GetPosition();
                            vec3 scale = m_CurrentEntity->GetScale();
                            btRigidBody* rigidBody = Physics::CreateRigidCube(btVector3(position.x, position.y, position.z), scale.x, scale.y, scale.z, 1.0f);
                            PhysicsComponent* component = new Bullet3Component(rigidBody);

                            if (m_CurrentEntity->AddComponent(component))
                            {
                                Physics::RegisterObject(rigidBody);
                            }
                            else
                            {
                                delete component;
                                delete rigidBody; // #TODO Add include file for type
                                break;
                            }

                            Bullet3Routine* bulletRoutine = new Bullet3Routine();
                            if (m_CurrentEntity->AddUpdateRoutine(bulletRoutine))
                            {
                                bulletRoutine->Initialize();
                            }
                            else
                            {
                                delete bulletRoutine;
                            }
                        }
                        break;
                    }

                    showComponentSelector = false;
                }
            }

            m_EditComponent->Draw(m_CurrentEntity);
        }

        ImGui::End();
	}

    static bool hasWarned = false;
    void InspectFieldRecursive(const Mirror::ClassInfo* classInfo, void* obj, std::string parentName)
    {
        const std::vector<Mirror::Field>& fields = classInfo->fields;

        for (size_t i = 0; i < fields.size(); i++)
        {
            const Mirror::Field field = fields[i];

            const void* fieldAddress = (void*)((char*)obj + field.offset);

            switch (field.typeInfo->enumType)
            {
            case MirrorTypes::m_vector_renderable:
            {
                std::vector<Renderable>* renderables = (std::vector<Renderable>*)fieldAddress;
                for (size_t i = 0; i < renderables->size(); i++)
                {
                    parentName += field.name + " ";
                    InspectFieldRecursive(Mirror::InfoForClass<Renderable>(), (Renderable*)renderables->data() + i, parentName);
                    parentName.clear();
                }
                break;
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
            //                 InspectFieldRecursive(Mirror::InfoForClass<Renderable>(), (Renderable*)renderables->data() + i, parentName);
            //                 parentName.clear();
            //             }
            //             break;
            //
            //         default:
            //             break;
            //         }
            //     }
            //     break;

            case MirrorTypes::m_vector_routinePtr:
                break;

            case MirrorTypes::m_map_eComponentTags_componentPtr:
                {
                    std::map<eComponentTags, Component*>& components = *(std::map<eComponentTags, Component*>*)fieldAddress;
                    for (auto it : components)
                    {
                        Component* component = it.second;

                        parentName += field.name + " ";
                        switch (component->GetTag()) // #TODO Look to handle sub classes more elegantly
                        {
                        case Component_Render:
                            InspectFieldRecursive(Mirror::InfoForClass<RenderComponent>(), (RenderComponent*)component, parentName);
                            break;

                        case Component_Camera:
                            InspectFieldRecursive(Mirror::InfoForClass<ComponentCamera>(), (ComponentCamera*)component, parentName);
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
                    ImGui::DragFloat2(fieldName.c_str(), vector2Address, .1f);
                }
                break;

            case MirrorTypes::Vector3:
                {
                    float* vector3Address = (float*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    ImGui::DragFloat3(fieldName.c_str(), vector3Address, .1f);
                }
                break;

            case MirrorTypes::m_string:
                {
                    const std::string* stringAddress = (const std::string*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    ImGui::LabelText(fieldName.c_str(), stringAddress->data());
                }
                break;

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
                {
                    const char* constCharPtrAddress = *(const char**)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    ImGui::LabelText(fieldName.c_str(), constCharPtrAddress);
                }
                break;

            case MirrorTypes::m_char:
            case MirrorTypes::eKeys:
                {
                    char* charPtrAddress = (char*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    // ImGui::LabelText(fieldName.c_str(), charPtrAddress);
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
                    }
                }
                break;

            case MirrorTypes::m_float:
                {
                    float* numberAddress = (float*)fieldAddress;
                    std::string fieldName = parentName + field.name;
                    ImGui::DragFloat(fieldName.c_str(), numberAddress, .1f);
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
                    ImGui::DragInt(fieldName.c_str(), numberAddress);
                }
                break;

            case MirrorTypes::m_int64_t:
            case MirrorTypes::m_uint64_t:
            {
                double* numberAddress = (double*)fieldAddress;
            }
            break;

            case MirrorTypes::m_double:
                {
                    double* numberAddress = (double*)fieldAddress;
                }
                break;

            default:
                if (field.classInfo)
                {
                    parentName += field.name + " ";
                    InspectFieldRecursive(field.classInfo, (char*)obj + field.offset, parentName);
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

    void EntityEditor::DrawEntityEditorInspect()
    {
        if (!m_CurrentEntity || !ImGui::Begin("Inspect GameObject"))
            return;

        std::string buffer = "";
        buffer.reserve(200);

        ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.5f); // #TODO Scale nicer
        InspectFieldRecursive(Mirror::InfoForClass<GameObject>(), m_CurrentEntity, buffer);
        ImGui::PopItemWidth();

        ImGui::End();
    }

}
