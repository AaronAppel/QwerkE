#include "QF_Serialization.h"

#include <map>
#include <vector>

#ifdef _QCJSON
#include "Libraries/cJSON/QC_cJSON.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#include "QC_Guid.h"

#include "QF_eKeys.h"
#include "QF_EntityHandle.h"
#include "QF_Log.h"
#include "QF_Settings.h"

#include "QF_ComponentCamera.h"
#include "QF_ComponentInfo.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

#define SERIALIZER_OPTIMIZATION_LEVEL 0 // Unsafe if serialization pattern changed, but much less redundant checking

// #TODO Look at removing from QwerkE namespace, or have a #using QwerkE just in this .cpp.
// Helps with porting to a stand alone library, and encourages type indifference/independence
namespace QwerkE {

	namespace Serialization {

		void local_DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj);
		void local_DeserializeJsonNumber(const cJSON* jsonObj, const MirrorTypes type, const unsigned int size, void* obj);
		void local_DeserializeJsonString(const cJSON* jsonObj, const MirrorTypes type, void* obj);

		void local_DeserializeFieldJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* fieldTypeInfo, void* fieldObj);

		void DeserializeJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, void* obj)
		{
			if (!objJson || !objTypeInfo || !obj)
			{
				LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
				return;
			}

			if (objTypeInfo->isPrimitive())
			{
				switch (objJson->type)
				{
				case cJSON_String:
					local_DeserializeJsonString(objJson, objTypeInfo->enumType, (char*)obj);
					break;

				case cJSON_True:
				case cJSON_False:
				case cJSON_Number:
					local_DeserializeJsonNumber(objJson, objTypeInfo->enumType, objTypeInfo->size, (char*)obj);
					break;

				default:
					// #TODO Handle reading/writing a simple primitive to a file
					LOG_WARN("{0} Unhandled early return for type {1}", __FUNCTION__, (int)objTypeInfo->enumType);
				}

				return; // Avoid non-primitive type logic below
			}

			if (objTypeInfo->fields.empty())
			{
				local_DeserializeFieldJsonToObject(objJson, objTypeInfo, obj);
				return;
			}

			std::vector<cJSON*> jsonStructArr = GetAllItemsFromArray(objJson); // #TODO Walk the linked list instead of creating a temporary vector?
			if (jsonStructArr.empty() && !objTypeInfo->isPrimitive() && objJson->type != cJSON_Array)
			{
				jsonStructArr = GetAllSiblingsWithObject(objJson);
			}

			if (jsonStructArr.size() != objTypeInfo->fields.size())
			{
				if (const bool nonInheritanceMismatch = !objTypeInfo->hasSubClass() && !objTypeInfo->isSubClass())
				{
					LOG_WARN("{0} Mismatched array sizes for json object {1} ", __FUNCTION__, objJson->string);
				}
			}

			for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
			{
				const Mirror::Field& field = objTypeInfo->fields[i];

#if SERIALIZER_OPTIMIZATION_LEVEL > 0
				for (size_t j = i; j < jsonStructArr.size(); j++)
				{
#else
				for (size_t j = 0; j < jsonStructArr.size(); j++)
				{
#endif
#if SERIALIZER_OPTIMIZATION_LEVEL < 2
					if (strcmp(field.name.c_str(), jsonStructArr[j]->string) != 0)
						continue;
#endif
					switch (jsonStructArr[j]->type)
					{
					case cJSON_String:
						local_DeserializeJsonString(jsonStructArr[j], field.typeInfo->enumType, (char*)obj + field.offset);
						break;

					case cJSON_True:
					case cJSON_False:
					case cJSON_Number:
						local_DeserializeJsonNumber(jsonStructArr[j], field.typeInfo->enumType, field.typeInfo->size, (char*)obj + field.offset);
						break;

					case cJSON_Array:
						local_DeserializeJsonArray(jsonStructArr[j], field, (char*)obj + field.offset);
						break;

					case cJSON_Object:
					case cJSON_NULL:
					default:
						LOG_WARN("{0} JSON type {1} not supported here!", __FUNCTION__, (int)jsonStructArr[j]->type);
						break;
					}

					break; // Next field
				}
			}
		}

		void local_DeserializeFieldJsonToObject(const cJSON* objJson, const Mirror::TypeInfo* fieldTypeInfo, void* fieldObj)
		{
			if (!objJson || !fieldTypeInfo || !fieldObj)
			{
				LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
				return;
			}

			switch (fieldTypeInfo->enumType)
			{
			case MirrorTypes::m_pair_guid_string:
				{
					using PairGuidString = std::pair<GUID, std::string>;
					PairGuidString* guidStringPair = (PairGuidString*)((char*)fieldObj);

					guidStringPair->first = std::stoll(objJson->string);
					guidStringPair->second = objJson->valuestring;
				}
				break;

			case MirrorTypes::m_vec_pair_guid_string:
				{
					using PairGuidString = std::pair<GUID, std::string>;
					std::vector<PairGuidString>* vecGuidStrings = (std::vector<PairGuidString>*)((char*)fieldObj);

					std::vector<cJSON*> pairsArray = GetAllItemsFromArray(objJson);

					const Mirror::TypeInfo* pairTypeInfo = Mirror::InfoForType<PairGuidString>();
					for (size_t i = 0; i < pairsArray.size(); i++)
					{
						PairGuidString pairGuidString;
						cJSON* pairJson = pairsArray[i]->child->child; // #TODO Shouldn't have to do ->child->child
						DeserializeJsonToObject(pairJson, pairTypeInfo, (void*)&pairGuidString);
						vecGuidStrings->push_back(pairGuidString);
					}
				}
				break;

			default:
				LOG_WARN("{0} Unsupported type", __FUNCTION__);
				break;
			}
		}

		template <class T>
		T* DeserializeSubclassPointer(const cJSON * jsonObj) // #TODO Look to handle sub classes more elegantly
		{
			T* indexPtrAddress = nullptr;

			switch (Mirror::InfoForType<T>()->enumType)
			{
			case MirrorTypes::Routine:
				switch ((eRoutineTypes)std::stoi(jsonObj->string))
				{

				default:
					break;
				}

				// Deserialize parent/derived values
				// Mirror::GetParentClassInfo<Routine>()
				// DeserializeJsonToObject(jsonObj, Mirror::InfoForType<Routine>(), indexPtrAddress);

				return indexPtrAddress;
			}
		}

		template <class T>
		void DeserializeVector(const cJSON * jsonObj, void* obj)
		{
			typedef std::remove_pointer_t<T> AbsoluteTypeT;

			const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);

			if (std::is_pointer_v<T>)
			{
				std::vector<AbsoluteTypeT*>* vectorPtrPtr = (std::vector<AbsoluteTypeT*>*)obj; // #TODO Review if possible to validate pointer value
				vectorPtrPtr->reserve(jsonObjectVector.size());
				vectorPtrPtr->resize(0);

				for (size_t i = 0; i < jsonObjectVector.size(); i++)
				{
					AbsoluteTypeT* objectPtr = new AbsoluteTypeT();

					DeserializeJsonToObject(jsonObjectVector[i], Mirror::InfoForType<AbsoluteTypeT>(), objectPtr);
					vectorPtrPtr->push_back(objectPtr);
				}
			}
			else
			{
				std::vector<AbsoluteTypeT>* vectorPtrPtr = (std::vector<AbsoluteTypeT>*)obj; // #TODO Review if possible to validate pointer value
				vectorPtrPtr->reserve(jsonObjectVector.size());
				vectorPtrPtr->resize(0);

				for (size_t i = 0; i < jsonObjectVector.size(); i++)
				{
					AbsoluteTypeT objectInstance;
					DeserializeJsonToObject(jsonObjectVector[i], Mirror::InfoForType<AbsoluteTypeT>(), &objectInstance);
					vectorPtrPtr->push_back(objectInstance);
				}
			}
		}

		// #TODO Look at using component enum instead of strings
#define DeserializeComponent(COMPONENT_TYPE, add) \
		if (strcmp(componentsJsonVector.at(j)->string, #COMPONENT_TYPE) == 0) \
		{ \
			if (add) entity.AddComponent<COMPONENT_TYPE>(); \
			COMPONENT_TYPE& component = entity.GetComponent<COMPONENT_TYPE>(); \
			DeserializeJsonToObject(componentsJsonVector[j], Mirror::InfoForType<COMPONENT_TYPE>(), (void*)&component); \
			continue; \
		}

		void local_DeserializeJsonArray(const cJSON* jsonObj, const Mirror::Field& field, void* obj)
		{
			if (!jsonObj || !jsonObj->type || jsonObj->type != cJSON_Array || !obj)
			{
				LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
				return;
			}

			switch (field.typeInfo->enumType)
			{
				// imgui types
			case MirrorTypes::m_imvec4_array:
			{
				if (field.typeInfo->isCollection())
				{
					ImVec4* colours = (ImVec4*)obj; // #TODO Review if possible to validate pointer value
					const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<ImVec4>();

					const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);

					for (size_t i = 0; i < jsonObjectVector.size(); i++)
					{
						DeserializeJsonToObject(jsonObjectVector[i], typeInfo, &colours[i]);
					}
				}
			}
			break;
			//

			case MirrorTypes::m_vec_pair_guid_string:
				{
					std::vector<std::pair<GUID, std::string>>* vecGuidStrings = (std::vector<std::pair<GUID, std::string>>*)obj;
					const std::vector<cJSON*> guidStringsJsonVector = GetAllItemsFromArray(jsonObj);

					vecGuidStrings->reserve(guidStringsJsonVector.size());
					for (size_t i = 0; i < guidStringsJsonVector.size(); i++)
					{
						using PairGuidString = std::pair<GUID, std::string>;
						PairGuidString pair;

						const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<PairGuidString>();
						DeserializeJsonToObject(guidStringsJsonVector[i], typeInfo, &pair);
						vecGuidStrings->push_back(pair);
					}
				}
				break;

			case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
				{
					std::unordered_map<eScriptTypes, Scriptable*>* scriptsMap = (std::unordered_map<eScriptTypes, Scriptable*>*)obj;
					scriptsMap->clear();

					// #TODO Get enums from json and assign to scripts map
					const std::vector<cJSON*> scriptsJsonVector = GetAllItemsFromArray(jsonObj);
					for (size_t i = 0; i < scriptsJsonVector.size(); i++)
					{
						const eScriptTypes scriptType = (eScriptTypes)std::stoi(scriptsJsonVector[i]->string);

						scriptsMap->insert({ scriptType, nullptr });

						switch (scriptType)
						{
						case eScriptTypes::Invalid:
							break;

						case eScriptTypes::Camera:
							break;

						case eScriptTypes::Testing:
							break;

						default:
							LOG_ERROR("{0} Unsupported script type!", __FUNCTION__);
							break;
						}
					}
				}
				break;

			case MirrorTypes::m_map_guid_entt:
				{
					std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)obj;
					entitiesMap->clear();
					Scene* scene = (Scene*)((char*)obj - field.offset);

					const std::vector<cJSON*> entitiesJsonVector = GetAllItemsFromArray(jsonObj);

					for (size_t i = 0; i < entitiesJsonVector.size(); i++)
					{
						const std::vector<cJSON*> componentsJsonVector = GetAllItemsFromArray(entitiesJsonVector[i]->child->child);

						GUID guid;
						if (strcmp(entitiesJsonVector[i]->string, "Entity") != 0)
						{
							guid = std::stoull(entitiesJsonVector[i]->string);
						}

						EntityHandle entity = scene->CreateEntity(guid);

						for (size_t j = 0; j < componentsJsonVector.size(); j++)
						{
							// #TODO Look at using component enum instead of strings
							DeserializeComponent(ComponentTransform, false) // #NOTE Added to every entity on creation
							DeserializeComponent(ComponentInfo, false)
							DeserializeComponent(ComponentCamera, true) // #NOTE Need to be added only if in data
							DeserializeComponent(ComponentMesh, true)
							DeserializeComponent(ComponentScript, true) // #TODO Consider using ComponentScript.AddScript()
						}

						ASSERT(entity.HasComponent<ComponentTransform>(), "Entity must have ComponentTransform!");
						ASSERT(entity.HasComponent<ComponentInfo>(), "Entity must have ComponentInfo!");

						if (entity.HasComponent<ComponentCamera>())
						{
							int bp = 0;
						}
					}
				}
				break;

			case MirrorTypes::m_floatArray16:
			{
				float* floatArray = (float*)obj;
				const std::vector<cJSON*> jsonObjectVector = GetAllItemsFromArray(jsonObj);
				for (size_t i = 0; i < jsonObjectVector.size(); i++)
				{
					floatArray[i] = jsonObjectVector[i]->valuedouble;
				}
			}
			break;

			case MirrorTypes::m_vector_string:
				// {
				//     // DeserializeVector<std::string>(jsonObj, obj); // #TODO Find out why std::string type is not supported
				//     // return;
				//
				//     std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj);
				//     const std::vector<cJSON*> jsonStrings = GetAllItemsFromArray(jsonObj);
				//     if (strings)
				//     {
				//         strings->reserve(jsonStrings.size());
				//         strings->resize(0);
				//
				//         for (size_t i = 0; i < jsonStrings.size(); i++)
				//         {
				//             // #TODO Handle more types agnosticly, so this logic would also support ints and other primitives/base values
				//             // #TODO This could be templated using the MirrorTypes enum if the function handles more than strings
				//             // DeserializeJsonString(jsonObj, MirrorTypes::m_string, obj);
				//
				//             strings->emplace_back(jsonStrings[i]->valuestring);
				//         }
				//     }
				// }
				DeserializeVector<std::string>(jsonObj, obj);
				break;

			default:
				DeserializeJsonToObject(jsonObj, field.typeInfo, obj);
				break;
			}
		}

		void local_DeserializeJsonNumber(const cJSON * jsonObj, const MirrorTypes type, const unsigned int typeSize, void* obj)
		{
			if (!jsonObj || !obj)
			{
				LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
				return;
			}

			if (jsonObj->type == cJSON_False && jsonObj->type == cJSON_True && jsonObj->type == cJSON_Number)
			{
				LOG_ERROR("{0} JSON object type is not a number or boolean!", __FUNCTION__);
				return;
			}

			switch (jsonObj->type)
			{
			case cJSON_True:
			case cJSON_False:
			{
				bool* fieldAddress = ((bool*)obj);
				*fieldAddress = jsonObj->type == cJSON_True;
			}
			break;

			case cJSON_Number:
			{
				void* fieldAddress = ((char*)obj);

				switch (type)
				{
				case MirrorTypes::m_float:
				{
					float* floatValuePtr = (float*)fieldAddress;
					*floatValuePtr = (float)jsonObj->valuedouble;
				}
				break;

				case MirrorTypes::m_double:
				{
					double* doubleValuePtr = (double*)fieldAddress;
					*doubleValuePtr = (double)jsonObj->valuedouble;
				}
				break;

				case MirrorTypes::m_uint64_t:
				{
					u64* valuePtr = (u64*)fieldAddress;
					const u64 temp = (u64)jsonObj->valuedouble;
					*valuePtr = temp;
				}
				break;

				default:
					void* sourceAddress = (void*)&jsonObj->valueint;
					memcpy(fieldAddress, sourceAddress, sizeof(jsonObj->valueint));
					break;
				}
			}
			break;

			case cJSON_Array:
			case cJSON_Object:
			case cJSON_NULL:
			default:
				// LOG_WARN("{0} JSON type {1} not supported here!", __FUNCTION__, (int)jsonStructArr[j]->type);
				LOG_WARN("Erorr in {0}", __FUNCTION__);
				break;
			}
		}

		void local_DeserializeJsonString(const cJSON * jsonObj, const MirrorTypes type, void* obj)
		{
			if (!jsonObj || !jsonObj->valuestring || (jsonObj->type != cJSON_String) || !obj)
			{
				LOG_ERROR("{0} Null/invalid argument passed!", __FUNCTION__);
				return;
			}

			switch (type)
			{
			case MirrorTypes::m_pair_guid_string:
				{
					int bp = 0;
				}
				break;

			case MirrorTypes::m_vec_pair_guid_string:
				{
					int bp = 0;
				}
				break;

			case MirrorTypes::m_string:
			{
				std::string* fieldAddress = (std::string*)((char*)obj);
				*fieldAddress = jsonObj->valuestring;
			}
			break;

			case MirrorTypes::eKeys:
			case MirrorTypes::m_char:
			{
				char* fieldAddress = (char*)obj;
				*fieldAddress = jsonObj->valuestring[0];
			}
			break;

			case MirrorTypes::m_charPtr:
			case MirrorTypes::m_constCharPtr:
			{
				char** fieldAddress = (char**)((char*)obj);
				*fieldAddress = (char*)_strdup(jsonObj->valuestring); // #TODO Handle malloc with matching free
			}
			break;

			default:
				// LOG_ERROR("{0} Unsupported field type {1} {2}({3}) for cJSON deserialization!", __FUNCTION__, field.name, field.type->stringName, (int)field.type->enumType);
				LOG_ERROR("Erorr in {0}", __FUNCTION__);
				break;
			}
		}

		template <class T>
		void SerializeVector(void* obj, cJSON * objJson)
		{
			typedef std::remove_pointer_t<T> AbsoluteTypeT;

			const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<AbsoluteTypeT>();

			if (std::is_pointer_v<T>)
			{
				const std::vector<AbsoluteTypeT*>* vectorPtrPtr = (std::vector<AbsoluteTypeT*>*)obj; // #TODO Review if possible to validate pointer value

				for (size_t i = 0; i < vectorPtrPtr->size(); i++)
				{
					if (!vectorPtrPtr->at(i))
						continue;

					cJSON* newJsonObjectArray = CreateArray(typeInfo->stringName.c_str());
					SerializeObjectToJson(vectorPtrPtr->at(i), typeInfo, newJsonObjectArray);
					AddItemToArray(objJson, newJsonObjectArray);
				}
			}
			else
			{
				const std::vector<AbsoluteTypeT>* vectorPtr = (std::vector<AbsoluteTypeT>*)obj; // #TODO Review if possible to validate pointer value

				for (size_t i = 0; i < vectorPtr->size(); i++)
				{
					cJSON* newJsonObjectArray = CreateArray(typeInfo->stringName.c_str());
					SerializeObjectToJson(&vectorPtr->at(i), typeInfo, newJsonObjectArray);
					AddItemToArray(objJson, newJsonObjectArray);
				}
			}
		}

#define SerializeComponent(COMPONENT_TYPE) \
		if (handle.HasComponent<COMPONENT_TYPE>()) \
		{ \
			auto& component = handle.GetComponent<COMPONENT_TYPE>(); \
			auto componentTypeInfo = Mirror::InfoForType<COMPONENT_TYPE>(); \
			cJSON* newJsonObjectArray = CreateArray(componentTypeInfo->stringName.c_str()); \
			SerializeObjectToJson(&component, componentTypeInfo, newJsonObjectArray); \
			AddItemToArray(entityComponentsJsonArray, newJsonObjectArray); \
		}

		void SerializeFieldToJson(const void* fieldObj, const Mirror::TypeInfo* fieldTypeInfo, cJSON* objJson);

		void SerializeObjectToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
		{
			if (!objJson || !objTypeInfo || !obj)
			{
				LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
				return;
			}

			if (objTypeInfo->fields.empty())
			{
				SerializeFieldToJson(obj, objTypeInfo, objJson);
			}

			for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
			{
				const Mirror::Field& field = objTypeInfo->fields[i];

				cJSON* arr = nullptr;
				if (!field.typeInfo->isPrimitive())
				{
					arr = CreateArray(field.name.c_str()); // #TODO Review using array for everything (like vec2), or JSON object (no'[]' braces)
					AddItemToArray(objJson, arr);
				}

				switch (field.typeInfo->enumType)
				{
				// imgui types
				case MirrorTypes::m_imvec4_array:
				{
					if (field.typeInfo->isArray())
					{
						const ImVec4* colours = (ImVec4*)((char*)obj + field.offset);
						const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<ImVec4>();

						// #TODO Get array size
						for (size_t i = 0; i < ImGuiCol_COUNT; i++)
						{
							cJSON* imvec4ArrayJson = CreateArray(typeInfo->stringName.c_str());
							AddItemToArray(arr, imvec4ArrayJson);
							SerializeObjectToJson(&colours[i], typeInfo, imvec4ArrayJson);
						}
					}
				}
				break;
				// imgui types

				// #TODO May need to move as this is a collection (field), not a complex type
				case MirrorTypes::m_map_eScriptTypes_ScriptablePtr:
					{
						const std::unordered_map<eScriptTypes, Scriptable*>* scriptsMap = (std::unordered_map<eScriptTypes, Scriptable*>*)((char*)obj + field.offset);

						for (auto& scriptTypeScriptablePair : *scriptsMap)
						{
							AddItemToArray(arr, CreateNumber(std::to_string(scriptTypeScriptablePair.first).c_str(), (u8)scriptTypeScriptablePair.first));
						}
					}
					break;

				case MirrorTypes::m_map_guid_entt:
					{
						// #NOTE m_map_guid_entt type can only be owned by a Scene. Look at enforcing through some static asserts of runtime cast checks
						const std::unordered_map<GUID, entt::entity>* entitiesMap = (std::unordered_map<GUID, entt::entity>*)((char*)obj + field.offset);
						Scene* scene = (Scene*)obj;

						for (auto& entityPair : *entitiesMap)
						{
							EntityHandle handle(scene, entityPair.second);

							// cJSON* entityJsonArray = CreateArray("Entity");
							cJSON* entityJsonArray = CreateArray(std::to_string(handle.EntityGuid()).c_str());
							AddItemToArray(arr, entityJsonArray);

							cJSON* entityComponentsJsonArray = CreateArray("Components");
							AddItemToArray(entityJsonArray, entityComponentsJsonArray);

							SerializeComponent(ComponentCamera)
							SerializeComponent(ComponentInfo)
							SerializeComponent(ComponentMesh)
							SerializeComponent(ComponentTransform)
							SerializeComponent(ComponentScript)
						}
					}
					break;

				case MirrorTypes::m_floatArray16:
				{
					float* floatArray = (float*)((char*)obj + field.offset);
					for (size_t i = 0; i < 16; i++)
					{
						AddItemToArray(arr, CreateNumber(std::to_string(i).c_str(), floatArray[i]));
					}
				}
				break;

				case MirrorTypes::m_vector_string: // #TODO Try to remove this case to use if (!objTypeInfo->fields.empty()) for loop
				{
					const std::vector<std::string>* strings = (std::vector<std::string>*)((char*)obj + field.offset);

					for (size_t i = 0; i < strings->size(); i++)
					{
						std::string str = strings->at(i);
						AddItemToArray(arr, CreateString(std::to_string(i).c_str(), str.c_str()));
					}
				}
				break;

				case MirrorTypes::m_string:
				{
					const std::string* fieldAddress = (const std::string*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress->data()));
				}
				break;

				case MirrorTypes::m_charPtr:
				case MirrorTypes::m_constCharPtr:
				{
					const char* fieldAddress = *(const char**)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateString(field.name.c_str(), fieldAddress));
				}
				break;

				case MirrorTypes::eKeys:
				case MirrorTypes::m_char:
				{
					char* stringAddress = (char*)obj + field.offset;
					AddItemToArray(objJson, CreateString(field.name.c_str(), stringAddress));
				}
				break;

				case MirrorTypes::m_bool:
				{
					bool* boolAddress = (bool*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateBool(field.name.c_str(), *boolAddress));
				}
				break;

				case MirrorTypes::m_float:
				{
					float* numberAddress = (float*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				case MirrorTypes::eScriptTypes: // #TODO Add a case for all enums by default
				case MirrorTypes::m_eSceneTypes:
				case MirrorTypes::m_uint8_t:
				{
					uint8_t* numberAddress = (uint8_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				case MirrorTypes::m_uint16_t:
				{
					uint16_t* numberAddress = (uint16_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				case MirrorTypes::m_uint32_t:
				{
					uint32_t* numberAddress = (uint32_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				case MirrorTypes::m_int8_t:
				{
					int8_t* numberAddress = (int8_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), (double)*numberAddress));
				}
				break;

				case MirrorTypes::m_int16_t:
				{
					int16_t* numberAddress = (int16_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				case MirrorTypes::m_int:
				case MirrorTypes::m_int32_t:
				{
					int32_t* numberAddress = (int32_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				case MirrorTypes::m_int64_t:
				{
					int64_t* numberAddress = (int64_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), (double)*numberAddress));
				}
				break;

				case MirrorTypes::m_uint64_t:
				{
					uint64_t* numberAddress = (uint64_t*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), (double)*numberAddress));
				}
				break;

				case MirrorTypes::m_double:
				{
					double* numberAddress = (double*)((char*)obj + field.offset);
					AddItemToArray(objJson, CreateNumber(field.name.c_str(), *numberAddress));
				}
				break;

				default:

					if (!field.typeInfo->fields.empty())
					{
						SerializeObjectToJson((char*)obj + field.offset, field.typeInfo, arr);
					}
					else
					{
						LOG_WARN("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, field.name, field.typeInfo->stringName, (int)field.typeInfo->enumType);
						break;
					}
				}
			}
		}

		void SerializeFieldToJson(const void* fieldObj, const Mirror::TypeInfo* fieldTypeInfo, cJSON* objJson)
		{
			if (!fieldObj || !fieldTypeInfo || !objJson)
			{
				LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
				return;
			}

			switch (fieldTypeInfo->enumType)
			{
			case MirrorTypes::m_pair_guid_string:
				{
					using PairGuidString = std::pair<GUID, std::string>;
					const PairGuidString* guidStringPair = (PairGuidString*)((char*)fieldObj);

					cJSON* pairArray = CreateArray(fieldTypeInfo->stringName.c_str());
					cJSON* pairEntry = CreateString(std::to_string(guidStringPair->first).c_str(), guidStringPair->second.c_str());
					AddItemToArray(pairArray, pairEntry);
					AddItemToArray(objJson, pairArray);
				}
				break;

			case MirrorTypes::m_vec_pair_guid_string:
				{
					using PairGuidString = std::pair<GUID, std::string>;
					const std::vector<PairGuidString>* vecGuidStrings = (std::vector<PairGuidString>*)((char*)fieldObj);

					for (size_t i = 0; i < vecGuidStrings->size(); i++)
					{
						const PairGuidString& pair = vecGuidStrings->at(i);

						const Mirror::TypeInfo* pairTypeInfo = Mirror::InfoForType<PairGuidString>();
						SerializeObjectToJson((void*)&pair, pairTypeInfo, objJson);
					}
				}
				break;

			default:
				LOG_WARN("{0} Unsupported type", __FUNCTION__);
				break;
			}
		}

	}

}