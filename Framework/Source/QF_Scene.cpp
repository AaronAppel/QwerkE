#include "QF_Scene.h"

#include <cstdint> // #REVIEW

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QDebug
#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bgfx/logo.h>
#include <bx/bx.h>
#include <bx/timer.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif
#endif
#endif

#include "QC_StringHelpers.h"

#include "QF_ComponentCamera.h"
#include "QF_Entity.h"
#include "QF_FileUtilities.h"
#include "QF_GameObject.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"
#include "QF_Window.h" // #TESTING

// #TODO Remove test code
#include "QF_ComponentPrint.h"
#include "QF_ScriptPrint.h"
#include "QF_NativeScript.h"

#include "Libraries/bgfx/include/bgfx/bgfx.h"

static void OnComponentCameraConstruct(entt::registry& registry, entt::entity entity)
{
}

// #TESTING
namespace QwerkE {

    namespace Renderer {

        const bgfx::ViewId s_ViewIdFbo1 = 2;

        extern bgfx::FrameBufferHandle s_FrameBufferHandle;
        extern const u8 s_FrameBufferTextureCount = 2;
        extern bgfx::TextureHandle s_FrameBufferTextures[s_FrameBufferTextureCount];
        extern bgfx::TextureHandle s_ReadBackTexture; // #TODO Destroy

    }

}
//

namespace QwerkE {

    static bgfx::VertexBufferHandle m_vbh;
    static bgfx::IndexBufferHandle m_ibh;
    static bgfx::ProgramHandle m_program;

    const int64_t m_StartingTimeOffset = bx::getHPCounter();

    Scene::Scene(const std::string& sceneFileName) :
        m_SceneFileName(sceneFileName)
    {
        m_Registry.on_construct<ComponentCamera>().connect<&OnComponentCameraConstruct>();

        m_EntityCamera = m_Registry.create();
        Entity* qwerkeEntity = new Entity(this, m_EntityCamera);
        const bool hasComponent = qwerkeEntity->HasComponent<ComponentCamera>();

        m_Entities.insert(std::pair(m_EntityCamera, qwerkeEntity));

        m_Registry.emplace<ComponentCamera>(m_EntityCamera, ComponentCamera());
        const bool nowHasComponent = qwerkeEntity->HasComponent<ComponentCamera>();

        qwerkeEntity->AddComponent<NativeScriptComponent>().Bind<ScriptablePrint>(qwerkeEntity);

        // m_Registry.emplace<ScriptablePrint>(m_EntityCamera, qwerkeEntity);

        entt::entity m_EntityScript = m_Registry.create();
        m_Registry.emplace<ComponentPrint>(m_EntityScript);

        m_Registry.emplace<ComponentPrint>(m_Registry.create());
        m_Registry.emplace<ComponentPrint>(m_Registry.create());

        // #TESTING
        // Create vertex stream declaration.
        PosColorVertex::init();

        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
            , PosColorVertex::ms_layout
        );
        m_ibh = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
        );

        // Create program from shaders.
        m_program = myLoadShaderProgram("vs_cubes.bin", "fs_cubes.bin");
}

    Scene::~Scene()
    {
        UnloadScene();

        auto viewScripts = m_Registry.view<NativeScriptComponent>();
        for (auto entity : viewScripts)
        {
            NativeScriptComponent& script = m_Registry.get<NativeScriptComponent>(entity);
            if (script.Instance)
            {
                script.OnDestroyFunction(script.Instance);
                script.DeleteFunction();
            }
        }

        for (auto& pair : m_Entities)
        {
            if (pair.second)
            {
                delete pair.second;
                pair.second = nullptr;
            }
        }
        m_Entities.clear();

        bgfx::destroy(m_ibh);
        bgfx::destroy(m_vbh);
        bgfx::destroy(m_program);

        // m_Registry.destroy(); // #TODO Deallocate entt
    }

    void Scene::Update(float deltaTime)
    {
        if (m_IsPaused)
            return;

        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
        {
            if (!script.Instance)
            {
                script.InstantiateFunction();
                script.OnCreateFunction(script.Instance);
            }
            // script.OnUpdateFunction(script.Instance, deltaTime);
        });

        auto view = m_Registry.view<ComponentCamera>();
        for (auto entity :view)
        {
            ComponentCamera& camera = m_Registry.get<ComponentCamera>(entity);
            camera.Move();
        }

        auto viewPrints = m_Registry.view<ComponentPrint>();
        bgfx::dbgTextPrintf(0, 5, 0x0f, "ComponentPrints #%i", viewPrints.size());

        for (auto object : m_pGameObjects)
        {
            object.second->Update(deltaTime);
        }
    }

    void Scene::Draw()
    {
#ifdef _QDebug // #TESTING

        const vec2& windowSize = Window::GetSize();

        bgfx::setViewName(Renderer::s_ViewIdFbo1, "FBO1");
        bgfx::setViewClear(Renderer::s_ViewIdFbo1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(Renderer::s_ViewIdFbo1, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));

        bgfx::setViewFrameBuffer(Renderer::s_ViewIdFbo1, Renderer::s_FrameBufferHandle);
        bgfx::touch(Renderer::s_ViewIdFbo1);

        DebugDrawEncoder debugDrawer; // #TESTING
        debugDrawer.begin(Renderer::s_ViewIdFbo1);

        const bx::Vec3 normal = { 0.f,  1.f, 0.f };
        const bx::Vec3 pos = { 0.f, -2.f, 15.f };

        bx::Plane plane(bx::InitNone);
        bx::calcPlane(plane, normal, pos);

        debugDrawer.drawGrid(Axis::Y, pos, 128, 1.0f);

        debugDrawer.end();

        {
            float time = (float)((bx::getHPCounter() - m_StartingTimeOffset) / double(bx::getHPFrequency()));

            const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
            const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

            // Set view and projection matrix for view 0.
            {
                float view[16];
                bx::mtxLookAt(view, eye, at);

                float proj[16];
                bx::mtxProj(proj, 60.0f, windowSize.x / windowSize.y, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
                bgfx::setViewTransform(Renderer::s_ViewIdFbo1, view, proj);

                // Set view 0 default viewport.
                // bgfx::setViewRect(Renderer::s_ViewIdFbo1, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));
            }

            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            bgfx::touch(0);

            uint64_t state = 0
                | BGFX_STATE_WRITE_R
                | BGFX_STATE_WRITE_G
                | BGFX_STATE_WRITE_B
                | BGFX_STATE_WRITE_A
                | BGFX_STATE_WRITE_Z
                | BGFX_STATE_DEPTH_TEST_LESS
                | BGFX_STATE_CULL_CW
                | BGFX_STATE_MSAA
                | 0 // #REVIEW
                ;

            // Submit 11x11 cubes.
            for (uint32_t yy = 0; yy < 11; ++yy)
            {
                for (uint32_t xx = 0; xx < 11; ++xx)
                {
                    float mtx[16];
                    bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
                    mtx[12] = -15.0f + float(xx) * 3.0f;
                    mtx[13] = -15.0f + float(yy) * 3.0f;
                    mtx[14] = 0.0f;

                    // Set model matrix for rendering.
                    bgfx::setTransform(mtx);

                    // Set vertex and index buffer.
                    bgfx::setVertexBuffer(0, m_vbh);
                    bgfx::setIndexBuffer(m_ibh);

                    // Set render states.
                    bgfx::setState(state);

                    // Submit primitive for rendering to view 0.
                    bgfx::submit(Renderer::s_ViewIdFbo1, m_program);
                }
            }
        }

        // bgfx::blit(Renderer::s_ViewIdFbo1, Renderer::s_ReadBackTexture, 10, 20, Renderer::s_ReadBackTexture, 0, 0, windowSize.x * 0.6, windowSize.y * 0.8);

        // bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(windowSize.x / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(windowSize.y / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
#endif
    }

    bool Scene::AddCamera(GameObject* camera)
    {
        m_CameraList.push_back(camera);
        return true;
    }

    void Scene::RemoveCamera(GameObject* camera)
    {
        if (m_CameraList.size() < 2)
        {
            LOG_ERROR("Unable to remove camera as 1 must remain");
            return;
        }

        for (unsigned int i = 0; i < m_CameraList.size(); i++)
        {
            if (m_CameraList.at(i) == camera)
            {
                m_CameraList.erase(m_CameraList.begin() + i);
                // #TODO Delete?
                return;
            }
        }
    }

    bool Scene::AddLight(GameObject* light)
    {
        // #TODO Insert in/by render order, or sort after all objects have been added

        m_LightList.push_back(light);
        m_SceneDrawList.push_back(light);
        return true;
    }

    void Scene::RemoveLight(GameObject* light)
    {
        if (m_LightList.size() < 2)
        {
            LOG_ERROR("Unable to remove light as 1 must remain"); // #BUG B0001
            return;
        }

        for (unsigned int i = 0; i < m_LightList.size(); i++)
        {
            if (m_LightList.at(i) == light)
            {
                m_LightList.erase(m_LightList.begin() + i);
                auto it = m_SceneDrawList.erase(m_SceneDrawList.begin() + i);
                // #TODO Delete object
                return;
            }
        }
    }

    bool Scene::ObjectWithNameExists(GameObject* object)
    {
        if (!object)
            return true;

        return m_pGameObjects.find(object->GetName()) != m_pGameObjects.end();
    }

    bool Scene::AddObjectToScene(GameObject* object)
    {
        if (object)
        {
            if (ObjectWithNameExists(object))
            {
                LOG_WARN("{0} Object with name \"{1}\" already exists in scene{2}", __FUNCTION__, object->GetName().c_str(), this->GetSceneName().c_str());
            }
            else
            {
                m_pGameObjects[object->GetName()] = object;
                AddObjectToSceneDrawList(object);
                object->OnSceneLoaded(this);
                SetDirty();
                return true;
            }
        }
        else
        {
            LOG_ERROR("{0} Object was null!", __FUNCTION__);
        }
        return false;
    }

    void Scene::RemoveObjectFromScene(GameObject* object)
    {
        if (m_pGameObjects.find(object->GetName().c_str()) != m_pGameObjects.end())
        {
            RemoveObjectFromSceneDrawList(object);
            m_pGameObjects.erase(object->GetName());
            delete object;
        }
    }

    Entity* Scene::CreateEntity()
    {
        entt::entity enttEntity = m_Registry.create();
        // #TODO Add anycomponents like a transform. Also look at a no-transform version for directors
        // Could take in an optional tag/enum value to assign
        Entity* entity = new Entity(this, enttEntity);
        m_Entities.insert(std::pair(enttEntity, entity));
        return m_Entities.find(enttEntity)->second;
    }

    GameObject* Scene::CreateNewObject()
    {
        GameObject* newObject = new GameObject(this);

        return newObject;
    }

    GameObject* Scene::CreateNewObjectFromSchematic(const char* const schematicFileName)
    {
        GameObject* newGameObject = new GameObject(this);
        Serialization::DeserializeJsonFromFile(SchematicsFolderPath(schematicFileName), *newGameObject);

        while (ObjectWithNameExists(newGameObject))
        {
            char* newName = NumberAppendOrIncrement(newGameObject->GetName().c_str());
            if (newName)
            {
                newGameObject->SetName(newName); // #TODO Handle memory allocations
                delete[] newName;
            }
            else
            {
                LOG_ERROR("{0} Unable to name new game object!", __FUNCTION__);
                delete newGameObject;
                return nullptr;
            }
        }

        newGameObject->OnSceneLoaded(this);
        AddObjectToScene(newGameObject);
        return newGameObject;
    }

    void Scene::SaveScene()
    {
        if (m_SceneFileName == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Unable to save null scene file name!", __FUNCTION__);
            return;
        }

        // Serialization::SerializeScene(*this, ScenesFolderPath(m_SceneFileName.c_str()));

        Serialization::SerializeObjectToFile(*this, ScenesFolderPath(m_SceneFileName.c_str()));
        LOG_INFO("{0} Scene file {1} saved", __FUNCTION__, ScenesFolderPath(m_SceneFileName.c_str()));
        m_IsDirty = false;
    }

    void Scene::LoadSceneFromFilePath(const char* otherSceneFilePath)
    {
        if (m_IsLoaded)
        {
            LOG_ERROR("{0} Scene already loaded!", __FUNCTION__);
            return;
        }

        if (!otherSceneFilePath || otherSceneFilePath == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Could not load scene data from null scene file path!", __FUNCTION__);
            return;
        }

        std::string oldName = m_SceneFileName; // #TODO Improve scene file name overwrite logic
        if (FileExists(otherSceneFilePath))
        {
            Serialization::DeserializeJsonFromFile(otherSceneFilePath, *this);
        }
        else if (FileExists(ScenesFolderPath(otherSceneFilePath)))
        {
            Serialization::DeserializeJsonFromFile(ScenesFolderPath(otherSceneFilePath), *this);
        }
        else
        {
            LOG_ERROR("{0} Could not find scene file path {1} to load", __FUNCTION__, otherSceneFilePath);
            return;
        }

        m_SceneFileName = oldName;

        // Serialization::DeserializeScene(ScenesFolderPath(otherSceneFilePath), *this);

        OnLoaded();

        LOG_TRACE("{0} \"{1}\" loaded from file", __FUNCTION__, otherSceneFilePath);

        m_IsLoaded = true;
        m_IsDirty = false;
    }

    void Scene::LoadScene()
    {
        if (m_IsLoaded)
        {
            LOG_ERROR("{0} Scene already loaded!", __FUNCTION__);
            return;
        }

        if (m_SceneFileName.c_str() == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("Unable to load null scene! sceneFileName value is \"{0}\"", Constants::gc_DefaultStringValue);
            return;
        }

        Serialization::DeserializeJsonFromFile(ScenesFolderPath(m_SceneFileName.c_str()), *this);

        // Serialization::DeserializeScene(ScenesFolderPath(m_SceneFileName.c_str()), *this);

        OnLoaded();

        LOG_TRACE("{0} \"{1}\" loaded", __FUNCTION__, m_SceneFileName.c_str());

        m_IsLoaded = true;
        m_IsDirty = false;
    }

    void Scene::UnloadScene()
    {
        if (!m_IsLoaded)
        {
            LOG_ERROR("{0} Scene is not loaded!", __FUNCTION__);
            return;
        }

        for (size_t i = 0; i < m_CameraList.size(); i++)
        {
            delete m_CameraList.at(i);
        }
        m_CameraList.clear();

        for (auto object : m_pGameObjects)
        {
            delete object.second;
        }
        m_pGameObjects.clear();

        m_IsLoaded = false;
        m_IsDirty = false;
        LOG_TRACE("{0} \"{1}\" unloaded", __FUNCTION__, m_SceneFileName.c_str());
    }

    void Scene::ReloadScene()
    {
        UnloadScene();
        LoadScene();
        LOG_TRACE("{0} \"{1}\" reloaded", __FUNCTION__, m_SceneFileName.c_str());
    }

    void Scene::OnLoaded() // #TODO Improve load logic. These callbacks are to resolve serialization effects
    {
        for (size_t i = 0; i < m_SceneDrawList.size(); i++)
        {
            m_SceneDrawList[i]->OnSceneLoaded(this);
        }
        for (size_t i = 0; i < m_CameraList.size(); i++)
        {
            m_CameraList[i]->OnSceneLoaded(this);
        }
        for (size_t i = 0; i < m_LightList.size(); i++)
        {
            m_LightList[i]->OnSceneLoaded(this);
        }

        for (size_t i = 0; i < m_SceneDrawList.size(); i++)
        {
            if (m_pGameObjects.find(m_SceneDrawList[i]->GetName()) == m_pGameObjects.end())
            {
                if (m_SceneDrawList[i]->GetTag() != eGameObjectTags::GO_Tag_Light)
                {
                    m_pGameObjects[m_SceneDrawList[i]->GetName()] = m_SceneDrawList[i];
                }
            }
        }
    }

    const GameObject* Scene::GetGameObject(const char* name)
    {
        if (m_pGameObjects.find(name) != m_pGameObjects.end())
            return m_pGameObjects[name];
        return nullptr;
    }

    bool Scene::AddObjectToSceneDrawList(GameObject* object)
    {
        // #TODO Sort by draw order
        m_SceneDrawList.push_back(object);
        return true;
    }

    bool Scene::RemoveObjectFromSceneDrawList(const GameObject* object)
    {
        for (auto it = m_SceneDrawList.begin(); it != m_SceneDrawList.end(); it++)
        {
            if (*it == object)
            {
                m_SceneDrawList.erase(it);
                return true;
            }
        }
        return false;
    }

}
