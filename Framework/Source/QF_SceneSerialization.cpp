#include "QF_Serialization.h"

#include <vector>

#ifdef _QCJSON
#include "Libraries/cJSON/QC_cJSON.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QF_Debug.h"
#include "QF_EntityHandle.h"
#include "QF_Log.h"
#include "QF_Scene.h"

#include "QF_ComponentCamera.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

	namespace Serialization {

#define SerializeComponent(COMPONENT_TYPE) \
		if (handle.HasComponent<COMPONENT_TYPE>()) \
		{ \
			auto& component = handle.GetComponent<COMPONENT_TYPE>(); \
			auto componentTypeInfo = Mirror::InfoForType<COMPONENT_TYPE>(); \
			cJSON* newJsonObjectArray = CreateArray(componentTypeInfo->stringName.c_str()); \
			SerializeObjectToJson(&component, componentTypeInfo, newJsonObjectArray); \
			AddItemToArray(entityComponentsJsonArray, newJsonObjectArray); \
		}

		void SerializeScene(Scene& scene, const char* const absoluteSceneJsonFilePath)
		{
			if (!absoluteSceneJsonFilePath)
			{
				LOG_ERROR("{0} Null file path given!", __FUNCTION__);
				return;
			}

			const Mirror::TypeInfo* sceneTypeInfo = Mirror::InfoForType<Scene>();

			cJSON* jsonRootObject = cJSON_CreateObject();
			cJSON* sceneArray = CreateArray(sceneTypeInfo->stringName.c_str());
			AddItemToArray(jsonRootObject, sceneArray);

			SerializeObjectToJson(&scene, sceneTypeInfo, sceneArray);

			cJSON* entitiesJsonArray = CreateArray("Entities");
			AddItemToArray(sceneArray, entitiesJsonArray);

			const auto& entities = scene.EntitiesMap();
			for (auto& entityPair : entities)
			{
				EntityHandle handle(&scene, entityPair.second);

				cJSON* entityJsonArray = CreateArray("Entity");
				AddItemToArray(entitiesJsonArray, entityJsonArray);

				cJSON* entityComponentsJsonArray = CreateArray("Components");
				AddItemToArray(entityJsonArray, entityComponentsJsonArray);

				SerializeComponent(ComponentTransform)
				SerializeComponent(ComponentMesh)
				SerializeComponent(ComponentCamera)
			}

			PrintRootObjectToFile(absoluteSceneJsonFilePath, jsonRootObject);
			cJSON_Delete(jsonRootObject);
		}

		void DeserializeScene(const char* const absoluteSceneJsonFilePath, Scene& scene)
		{
			if (!absoluteSceneJsonFilePath)
			{
				LOG_ERROR("{0} Null file path given!", __FUNCTION__);
				return;
			}

			// #TODO Access registry as friend class or some better way than a public getter
			// entt::registry& registry = scene.Registry();

			const Mirror::TypeInfo* sceneTypeInfo = Mirror::InfoForType<Scene>();
			if (cJSON* rootJsonObject = OpencJSONStream(absoluteSceneJsonFilePath))
			{
				if (!rootJsonObject->child)
				{
					LOG_ERROR("{0} root JSON object has no children in JSON file {1}!", __FUNCTION__, absoluteSceneJsonFilePath);
				}
				else if (strcmp(rootJsonObject->child->string, sceneTypeInfo->stringName.c_str()) != 0)
				{
					LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, sceneTypeInfo->stringName.c_str());
				}
				else
				{
					std::vector<cJSON*> entitiesList = GetAllItemsFromArray(rootJsonObject->child);

					for (size_t i = 0; i < entitiesList.size(); i++)
					{
						EntityHandle entity = scene.CreateEntity();

						std::vector<cJSON*> componentsList = GetAllItemsFromArray(entitiesList[i]->child->child);

						for (size_t j = 0; j < componentsList.size(); j++)
						{
							// #TODO Look at using a macro for convenience
							// #TODO Look at using component enum instead of strings
							if (strcmp(componentsList.at(j)->string, "ComponentTransform") == 0)
							{
								ASSERT(entity.HasComponent<ComponentTransform>(), "Entity must have ComponentTransform!");
								ComponentTransform& transform = entity.GetComponent<ComponentTransform>();
								DeserializeJsonToObject(componentsList[j], Mirror::InfoForType<ComponentTransform>(), (void*)&transform);
							}
							if (strcmp(componentsList.at(j)->string, "ComponentMesh") == 0)
							{
								ComponentMesh& mesh = entity.AddComponent<ComponentMesh>();
								DeserializeJsonToObject(componentsList[j], Mirror::InfoForType<ComponentMesh>(), (void*)&mesh);
								mesh.Create();
								int bp = 0;
							}
						}
					}
				}

				ClosecJSONStream(rootJsonObject);
			}
			else
			{
				LOG_ERROR("{0} Could not load object type {1} from file {2}!", __FUNCTION__, sceneTypeInfo->stringName.c_str(), absoluteSceneJsonFilePath);
			}
		}

	}

}