#include "QF_Serialization.h"

#include <vector>

#include "Libraries/entt/entt.hpp"

#include "QF_Debug.h"
#include "QF_Scene.h"

namespace QwerkE {

	namespace Serialization {

		template <typename T>
		const T* SerializeEnttComponent(const entt::registry* const registry, const entt::entity& entity)
		{
			if (registry->has<T>(entity))
			{
				return &registry->get<T>(entity);
			}
			return nullptr;
		}

		void SerializeScene(const Scene& scene, const char* const absoluteSceneJsonFilePath)
		{
			const Mirror::TypeInfo* sceneTypeInfo = Mirror::InfoForType<Scene>();

			// Step 1 : Find entities vector
			std::vector<entt::entity>* entities = nullptr;
			for (size_t i = 0; i < sceneTypeInfo->fields.size(); i++)
			{
				const Mirror::Field& field = sceneTypeInfo->fields[i];

				if (field.typeInfo->enumType == MirrorTypes::m_vector_entt_entities)
				{
					void* entitiesVectorAddress = ((char*)&scene) + field.offset;
					entities = (std::vector<entt::entity>*)(entitiesVectorAddress);
					break;
				}
			}

			ASSERT(entities != nullptr, "Could not find field info for entt entities list!");
			entities = new (entities) std::vector<entt::entity>;

			// Step 2 : Find registry
			entt::registry* registry = nullptr;
			for (size_t i = 0; i < sceneTypeInfo->fields.size(); i++)
			{
				const Mirror::Field& field = sceneTypeInfo->fields[i];

				if (field.typeInfo->enumType == MirrorTypes::m_entt_registry)
				{
					void* registryAddress = ((char*)&scene) + field.offset;
					registry = (entt::registry*)(registryAddress);
					break;
				}
			}

			ASSERT(registry != nullptr, "Could not find field info for entt registry list!");

			registry = new (registry) entt::registry;

			registry->each([&](auto entityId)
				{
					entt::entity currentEntity = entityId;

					// Determine which components an entity has and serialize component data
					if (auto component = SerializeEnttComponent<int>(registry, currentEntity))
					{

					}

					// Get GameObject save data for component data

					// Build GameObject from file data, then tell registry about it's components
					const bool entityDataHasComponentInt = false;
					if (entityDataHasComponentInt)
					{
						int component = registry->emplace<int>(currentEntity);
					}

				});

			Serialization::SerializeObjectToFile(scene, absoluteSceneJsonFilePath);
		}

		void DeserializeScene(const char* const absoluteSceneJsonFilePath, Scene& scene)
		{
			const Mirror::TypeInfo* sceneTypeInfo = Mirror::InfoForType<Scene>();

			// Step 1 : Find registry
			entt::registry* registry = nullptr;
			for (size_t i = 0; i < sceneTypeInfo->fields.size(); i++)
			{
				const Mirror::Field& field = sceneTypeInfo->fields[i];

				if (field.typeInfo->enumType == MirrorTypes::m_entt_registry)
				{
					void* registryAddress = ((char*)&scene) + field.offset;
					registry = (entt::registry*)(registryAddress);
					break;
				}
			}
			ASSERT(registry != nullptr, "Could not find field info for entt registry list!");

			registry = new (registry) entt::registry;

			registry->each([&](auto entityId)
			{
					entt::entity currentEntity = entityId;

					// Get GameObject save data for component data

					// Build GameObject from file data, then tell registry about it's components
					const bool entityDataHasComponentInt = false;
					if (entityDataHasComponentInt)
					{
						int component = registry->emplace<int>(currentEntity);
					}

			});

			cJSON* jsonRootObject = cJSON_CreateObject();
			// cJSON* jsonRootArray = CreateArray(Mirror::InfoForType<T>()->stringName.c_str());
			// AddItemToObject(jsonRootObject, jsonRootArray);

			// Step 3 : Start adding entities and their components from file data
			// for (size_t i = 0; i < entities->size(); i++)
			{
			}

			// Step 4 : Serialize all other (non entt) Scene fields
			Serialization::DeserializeJsonFromFile(absoluteSceneJsonFilePath, scene);
		}

	}

}