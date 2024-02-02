#include "QC_cJSON.h"

#include <vector>

#include "cJSON.h"

// #TODO This file should be a library and can't include engine code, or be moved to become engine code
#include "QF_Log.h"
#include "QF_FileUtilities.h"

cJSON* OpencJSONStream(const char* fileDirectory)
{
	if (json_FileExists(fileDirectory))
	{
		char* str = LoadFile(fileDirectory);
		cJSON* root = cJSON_Parse(str);

		if (root == nullptr)
		{
			LOG_ERROR("{0}(): Could not open cJSON stream. Possible compile error. Check {1} file for typos!", __FUNCTION__, fileDirectory);
		}

		delete[] str;
		return root;
	}
	LOG_ERROR("{0}(): Could not find JSON file {1}!", __FUNCTION__, fileDirectory);
	return nullptr;
}

void ClosecJSONStream(const cJSON* root)
{
	if (root)
	{
		cJSON_Delete((cJSON*)root);
	}
}

std::vector<cJSON*> GetAllItemsFromObject(const cJSON* cJSONObject)
{
	int arraySize = GetObjectSize(cJSONObject);

	std::vector<cJSON*> returnList;

	for (int i = 0; i < arraySize; i++)
	{
		returnList.push_back(cJSON_GetArrayItem((cJSON*)cJSONObject, i));
	}
	return returnList;
}

std::vector<cJSON*> GetItemsFromObjectByKey(cJSON* cJSONObject, const char* key)
{
	int arraySize = cJSON_GetArraySize(cJSONObject);

	std::vector<cJSON*> returnList;

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem(cJSONObject, i);
		if (*item->string == *key)
		{
			returnList.push_back(item);
		}
	}
	return returnList;
}

std::vector<cJSON*> GetItemsFromObjectByString(cJSON* cJSONObject, const char* value)
{
	int arraySize = cJSON_GetArraySize(cJSONObject);

	std::vector<cJSON*> returnList;

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem(cJSONObject, i);
		if (item->valuestring == nullptr)
		{
			continue;
		}
		if (*item->valuestring == *value)
		{
			returnList.push_back(item);
		}
	}
	return returnList;
}

std::vector<cJSON*> GetItemsFromObjectByIndexRange(cJSON* cJSONObject, int minimum, int maximum)
{
	std::vector<cJSON*> returnList;

	int range = maximum - minimum + 1;

	for (int i = 0; i < range; i++)
	{
		returnList.push_back(cJSON_GetArrayItem(cJSONObject, minimum + i));
	}
	return returnList;
}

cJSON* GetItemFromObjectByKey(const cJSON* cJSONObject, const char* key)
{
	if (!cJSONObject)
		return nullptr;

	int arraySize = cJSON_GetArraySize((cJSON*)cJSONObject);

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem((cJSON*)cJSONObject, i);
		if (strcmp(item->string, key) == 0)
		{
			return item;
		}
	}
	return nullptr;
}

cJSON* GetItemFromObjectByString(const cJSON* cJSONObject, const char* value)
{
	int arraySize = cJSON_GetArraySize((cJSON*)cJSONObject);

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem((cJSON*)cJSONObject, i);
		if (item->valuestring == nullptr)
		{
			continue;
		}
		if (*item->valuestring == *value)
		{
			return item;
		}
	}
	return nullptr;
}

cJSON* GetItemFromObjectByIndex(const cJSON* cJSONObject, int index)
{
	return cJSON_GetArrayItem((cJSON*)cJSONObject, index);
}

std::vector<cJSON*> GetAllItemsFromArray(const cJSON* arrayObject) // return array of objects inside of an objectArray
{
	std::vector<cJSON*> itemList;
	if (!arrayObject || !arrayObject->child)
		return itemList;

	const int t_ArraySize = cJSON_GetArraySize(arrayObject->child);
	itemList.reserve(t_ArraySize);
	for (int i = 0; i < t_ArraySize; i++)
	{
		itemList.push_back(cJSON_GetArrayItem(arrayObject->child, i));
	}

	return itemList;
}

std::vector<cJSON*> GetItemsFromArrayByKey(const cJSON* jObjectArray, const char* key) // #TODO Test
{
	const int t_ArraySize = cJSON_GetArraySize(jObjectArray->child);

	std::vector<cJSON*> itemList;
	itemList.reserve(t_ArraySize);

	for (int i = 0; i < t_ArraySize; i++)
	{
		cJSON* temp = cJSON_GetArrayItem(jObjectArray->child, i);
		if (*temp->string == *key) // #TODO Review false comparison
		{
			itemList.push_back(temp);
		}
	}

	return itemList;
}

std::vector<cJSON*> GetItemsFromArrayByString(const cJSON* cJSONArray, const char* value)
{
	int arraySize = cJSON_GetArraySize(cJSONArray->child);

	std::vector<cJSON*> returnList;

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem(cJSONArray->child, i);
		if (item->valuestring == nullptr)
		{
			continue;
		}
		if (*item->valuestring == *value)
		{
			returnList.push_back(item);
		}
	}
	return returnList;
}

std::vector<cJSON*> GetItemsFromArrayByIndexRange(const cJSON* cJSONObject, int min, int max)
{
	std::vector<cJSON*> returnList;

	int range = max - min + 1;

	cJSON* item = cJSONObject->child->child; // get first item

	for (int i = 0; i < min; i++)
	{
		item = item->next;
	}

	for (int i = 0; i < range; i++)
	{
		returnList.push_back(item);
		item = item->next;
	}

	return returnList;
}

cJSON* GetItemFromArrayByKey(const cJSON* cJSONArray, const char* key) // TODO:: BROKEN // <-- deprecated
{
	int arraySize = cJSON_GetArraySize(cJSONArray->child);

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem(cJSONArray->child, i);
		if (strcmp(item->string, key) == 0) // #TODO:: Test for bugs
		{
			return item;
		}
		if (*item->string == *key)
		{
			//return item; // #TODO:: Properly remove
		}
	}
	return nullptr;
}

cJSON* GetItemFromArrayByString(const cJSON* cJSONArray, const char* value) // #TODO Deprecate
{
	int arraySize = cJSON_GetArraySize(cJSONArray->child);

	for (int i = 0; i < arraySize; i++)
	{
		cJSON* item = cJSON_GetArrayItem(cJSONArray->child, i);
		if (item->valuestring == nullptr)
		{
			continue;
		}
		if (*item->valuestring == *value)
		{
			return item;
		}
	}
	return nullptr;
}

cJSON* GetItemFromArrayByIndex(const cJSON* cJSONarray, int index)
{
	cJSON* cJSONItem = cJSONarray->child->child;

	for (int i = 0; i < index; i++)
	{
		cJSONItem = cJSONItem->next;
	}
	return cJSONItem;
}

void AddItemToObject(cJSON* cJSONObject, cJSON* item)
{
	cJSON_AddItemToArray(cJSONObject, item);
}

void AddItemToArray(cJSON* cJSONArray, cJSON* item)
{
	int arraySize = 0;

	if (cJSONArray->child)
	{
		arraySize = cJSON_GetArraySize(cJSONArray->child);
	}
	else
	{
		cJSONArray->child = cJSON_CreateObject();
	}

	if (arraySize > 0)
	{
		cJSON* lastSibling = GetItemFromArrayByIndex(cJSONArray, arraySize - 1);
		lastSibling->next = item;
		item->prev = lastSibling;
	}
	else
	{
		if (cJSONArray->child->child == nullptr)
		{
			cJSONArray->child->child = item; // First element
		}
	}
}

cJSON* CreateBool(const char* key, bool value)
{
	cJSON* returnBool = cJSON_CreateBool(value);
	returnBool->string = _strdup(key); // #TODO Check for redundant memory allocations
	return returnBool;
}

cJSON* CreateInt(const char* key, double value)
{
	cJSON* returnNumber = cJSON_CreateNumber(value);
	returnNumber->string = _strdup(key);
	return returnNumber;
}

cJSON* CreateNumber(const char* key, double value)
{
	cJSON* returnNumber = cJSON_CreateNumber(value);
	returnNumber->string = _strdup(key);
	return returnNumber;
}

cJSON* CreateString(const char* key, const char* value)
{
	cJSON* returnString = cJSON_CreateString(value);
	returnString->string = _strdup(key);
	return returnString;
}

cJSON* CreateArray(const char* key)
{
	cJSON* returnArray = cJSON_CreateArray();
	returnArray->child = cJSON_CreateObject();
	returnArray->string = _strdup(key);
	return returnArray;
}

cJSON* CopyRootObject(cJSON* root)
{
	char* newRoot = cJSON_Print(root); // #TODO Handle memory allocation
	return cJSON_Parse(newRoot);
}

unsigned int GetObjectSize(const cJSON* cJSONObject)
{
	return cJSON_GetArraySize((cJSON*)cJSONObject);
}

unsigned int GetArraySize(const cJSON* cJSONArray)
{
	return cJSON_GetArraySize(cJSONArray->child);
}

cJSON* DeepSearchForItemByKey(cJSON* object, const char* key)
{
	cJSON* search = object->child;

	while (search)
	{
		if (search->string)
		{
			if (*search->string == *key)
			{
				return search;
			}
		}

		if (search->next)
		{
			std::vector<cJSON*> siblings;

			cJSON* siblingObj = search->next;

			while (siblingObj)
			{
				siblings.push_back(siblingObj);
				siblingObj = siblingObj->next;
			}

			for (cJSON* temp : siblings)
			{
				cJSON* variable;
				variable = DeepSearchForItemByKey(temp, key);
				if (variable)
				{
					if (variable->string)
					{
						if (*variable->string == *key)
						{
							return variable;
						}
					}
				}
			}
		}
		search = search->child;
	}
	return nullptr;
}

//*- Utility Helpers -*
bool json_FileExists(const char* filename) // #TODO Move to helpers.h/.cpp
{
	FILE* filehandle;
	fopen_s(&filehandle, filename, "r"); // #TODO Handle error
	// errno_t error = fopen_s(&filehandle, filename, "r");

	if (filehandle)
	{
		fclose(filehandle);
		return true;
	}
	else
	{
		// #TODO Handle error
		// OutputPrint("\nFileExists(): Could not find file. Ensure %s exists!\n\n", filename);
		return false;
	}
}

void json_CreateNewFile(const char* filename)
{
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filename, "r");

	if (error == 0)
	{
		LOG_ERROR("CreateNewFile(): {0} already exists", filename);
		fclose(filehandle);
	}
	else if (filehandle == nullptr)
	{
		errno_t error = fopen_s(&filehandle, filename, "w+");
		fclose(filehandle);
	}
}

void CreateEmptycJSONFile(const char* filePath)
{
	FILE* f;
	fopen_s(&f, filePath, "w+");
	if (f)
	{
		char buffer[] = { '{', '\n', '}' };
		fwrite(buffer, sizeof(char), sizeof(buffer), f);
		fclose(f);
	}
}

void AddItemToRootOfFile(cJSON* item, const char* filename)
{
	cJSON* root = OpencJSONStream(filename);
	if (root)
	{
		AddItemToRoot(root, item);
		PrintRootObjectToFile(filename, root);
		ClosecJSONStream(root);
	}
}

void PrintRootObjectToFile(const char* filename, cJSON* root)
{
	const char* jSonstr = cJSON_Print(root);
	WriteStringToFile(filename, jSonstr);
	free((char*)jSonstr);
}
