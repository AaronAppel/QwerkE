#include "QF_PBR_Test1.h"

#include "QF_Enums.h"
#include "QF_GameObject.h"
#include "QF_Factory.h"
#include "QF_Mesh.h"
#include "QF_MeshFactory.h"
#include "QF_Resources.h"
#include "QF_Serialization.h"
#include "QF_ShaderProgram.h"

namespace QwerkE {

    PBR_Test1::PBR_Test1() : Scene("PBR_Test1.qscene")
    {
        m_ID = (eSceneTypes::Scene_PBR_Test1);
    }

    void PBR_Test1::Initialize() // #TODO Move to a data file
    {
        Resources::GetShaderProgram("PBR1.ssch");
        // Resources::GetShaderProgram("test_normal.ssch");

        GameObject* newCamera = new GameObject(this);
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Camera.osch"), *newCamera);
        newCamera->SetPosition(vec3(2, 0, -15));
        newCamera->SetRotation(vec3(0, 45, 0));
        newCamera->OnSceneLoaded(this);
        this->AddCamera(newCamera);
        Scene::SetupCameras();

        for (int i = 0; i < 10; i++)
        {
            GameObject* newCube = new GameObject();
            Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Cube_UVd.osch"), *newCube);
            newCube->SetPosition(vec3(i - 5, i - 5, i));
            this->AddObjectToScene(newCube);

            GameObject* newSphere = new GameObject();
            Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Sphere.osch"), *newSphere);
            newSphere->SetPosition(vec3(i - 5, i - 5, i));
        }

        GameObject* newGameObject = new GameObject();
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Plane.osch"), *newGameObject);
        this->AddObjectToScene(newGameObject);

        GameObject* plane = new GameObject();
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Plane.osch"), *plane);
        plane->SetPosition(vec3(0, 0, 20));
        plane->SetScale(vec3(10, 10, 10));
        plane->SetRotation(vec3(90, 0, 0));
        this->AddObjectToScene(plane);

        GameObject* plane2 = new GameObject();
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Plane.osch"), *plane2);
        plane->SetPosition(vec3(2, 0, -5));
        plane->SetRotation(vec3(90, 0, 0));

        GameObject* newLight = new GameObject(this);
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Light.osch"), *newLight);
        newLight->SetPosition(vec3(2, 0, -5));
        newLight->OnSceneLoaded(this);
        this->AddLight(newLight);

        GameObject* newCube = new GameObject();
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Cube_UVd.osch"), *newCube);
        newCube->SetPosition(vec3(0, 0, -5));

        GameObject* newSphere = new GameObject();
        Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Sphere.osch"), *newSphere);
        newSphere->SetPosition(vec3(-2, 0, -5));
    }

    void PBR_Test1::Update(float deltatime)
    {
        for (size_t i = 0; i < m_Subjects.size(); i++)
        {
            const vec3 rotation = vec3(m_Subjects[i]->GetRotation().x, (float)m_Subjects[i]->GetRotation().y + 45.0f * (float)deltatime, m_Subjects[i]->GetRotation().z);
            m_Subjects[i]->SetRotation(rotation);

            if (m_Subjects[i]->GetRotation().y >= 360.0f) // #TODO What about < 0?
            {
                // #TODO Improve logic and variable naming
                const vec3 rotation2 = vec3(vec3(m_Subjects[i]->GetRotation().x, m_Subjects[i]->GetRotation().y - 360.0f, m_Subjects[i]->GetRotation().z));
                m_Subjects[i]->SetRotation(rotation2);
            }
        }
    }

}
