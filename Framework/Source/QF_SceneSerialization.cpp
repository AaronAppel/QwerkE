#include "QF_Serialization.h"

#include <vector>

#ifdef _QCJSON
#include "Libraries/cJSON/QC_cJSON.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Scene.h"

#include "QF_ComponentMesh.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

	namespace Serialization {

		void SerializeScene(const Scene& scene, const char* const absoluteSceneJsonFilePath)
		{
			if (!absoluteSceneJsonFilePath)
			{
				LOG_ERROR("{0} Null file path given!", __FUNCTION__);
				return;
			}

			const Mirror::TypeInfo* sceneTypeInfo = Mirror::InfoForType<Scene>();

			cJSON* jsonRootObject = cJSON_CreateObject();
			cJSON* jsonRootArray = CreateArray(sceneTypeInfo->stringName.c_str());
			AddItemToObject(jsonRootObject, jsonRootArray);

			u8 registryOffset = 0;
			for (size_t i = 0; i < sceneTypeInfo->fields.size(); i++)
			{
				if (strcmp(sceneTypeInfo->fields[i].name.c_str(), "m_Registry") == 0)
				{
					registryOffset = sceneTypeInfo->fields[i].offset;
					break;
				}
			}

			entt::registry* registry = (entt::registry*)((char*)&scene + registryOffset);

			std::vector<entt::entity> entitiesVec;

			registry->each([&](entt::entity entity)
				{
					// entitiesVec.push_back(entity);

					cJSON* entityJsonArray = CreateArray("Entity");
					AddItemToArray(jsonRootArray, entityJsonArray);

					cJSON* entityComponentsJsonArray = CreateArray("Components");
					AddItemToArray(entityJsonArray, entityComponentsJsonArray);

					// #TODO Could macro same logic functions
					if (registry->has<ComponentTransform>(entity))
					{
						auto& transform = registry->get<ComponentTransform>(entity);
						auto componentTypeInfo = Mirror::InfoForType<ComponentTransform>();
						cJSON* newJsonObjectArray = CreateArray(componentTypeInfo->stringName.c_str());
						SerializeObjectToJson(&transform, componentTypeInfo, newJsonObjectArray);
						AddItemToArray(entityComponentsJsonArray, newJsonObjectArray);
					}
					if (registry->has<ComponentMesh>(entity))
					{
						auto mesh = registry->get<ComponentMesh>(entity);
						auto componentTypeInfo = Mirror::InfoForType<ComponentMesh>();
						cJSON* newJsonObjectArray = CreateArray(componentTypeInfo->stringName.c_str());
						SerializeObjectToJson(&mesh, componentTypeInfo, newJsonObjectArray);
						AddItemToArray(entityComponentsJsonArray, newJsonObjectArray);
					}
				});

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
			entt::registry& registry = scene.Registry();

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
						entt::entity entityId = registry.create();
						std::vector<cJSON*> componentsList = GetAllItemsFromArray(entitiesList[i]->child->child);

						for (size_t j = 0; j < componentsList.size(); j++)
						{
							// #TODO Look at using a macro for convenience
							if (strcmp(componentsList.at(j)->string, "ComponentTransform") == 0)
							{
								ComponentTransform& transform = registry.emplace<ComponentTransform>(entityId);
								DeserializeJsonToObject(componentsList[j], Mirror::InfoForType<ComponentTransform>(), (void*)&transform);
							}
							if (strcmp(componentsList.at(j)->string, "ComponentMesh") == 0)
							{
								ComponentMesh& mesh = registry.emplace<ComponentMesh>(entityId);
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