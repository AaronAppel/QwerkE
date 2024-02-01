#pragma once

#include <vector>

#include "cJSON.h"

#define GetAllRootItems(root) GetAllItemsFromObject(root)
#define GetItemsFromRootByKey(root, key) GetItemsFromObjectByKey(root, key)
#define GetItemsFromRootByString(root, value) GetItemsFromObjectByString(root, value)
#define GetItemsFromRootByIndexRange(root, minimum, maximum) GetItemsFromObjectByIndexRange(root, minimum, maximum)

#define GetItemFromRootByKey(root, key) GetItemFromObjectByKey(root, key)
#define GetItemFromRootByString(root, value) GetItemFromObjectByString(root, value)

#define AddItemToRoot(root, item) AddItemToObject(root, item)

cJSON* OpencJSONStream(const char* fileDirectory);
void ClosecJSONStream(const cJSON* root);

void AddItemToRootOfFile(cJSON* object, char* filename);
void PrintRootObjectToFile(const char* filename, cJSON* root);

std::vector<cJSON*> GetAllItemsFromObject(const cJSON* cJSONObject);
std::vector<cJSON*> GetItemsFromObjectByKey(cJSON* cJSONObject, const char* key);
std::vector<cJSON*> GetItemsFromObjectByString(cJSON* cJSONObject, char* value);
std::vector<cJSON*> GetItemsFromObjectByIndexRange(cJSON* cJSONObject, int min, int max);

cJSON* GetItemFromObjectByKey(const cJSON* cJSONObject, const char* key);
cJSON* GetItemFromObjectByString(const cJSON* cJSONObject, const char* value);
cJSON* GetItemFromObjectByIndex(const cJSON* cJSONObject, int index);

std::vector<cJSON*> GetAllItemsFromArray(const cJSON* jItem);
std::vector<cJSON*> GetItemsFromArrayByKey(const cJSON* jObjectArray, char* key);
std::vector<cJSON*> GetItemsFromArrayByString(const cJSON* jObjectArray, char* value);
std::vector<cJSON*> GetItemsFromArrayByIndexRange(const cJSON* root, int min, int max);

cJSON* GetItemFromArrayByKey(const cJSON* jObjectArray, const char* targetObjectName);
cJSON* GetItemFromArrayByString(const cJSON* jObjectArray, const char* value);
cJSON* GetItemFromArrayByIndex(const cJSON* jObjectArray, int index);

void AddItemToObject(cJSON* cJSONObject, cJSON* item);
void AddItemToArray(cJSON* cJSONArray, cJSON* item);

cJSON* CreateBool(const char* key, bool value);
cJSON* CreateNumber(const char* key, double value);
cJSON* CreateString(const char* key, const char* value);
cJSON* CreateArray(const char* key);

bool json_FileExists(const char* filename);
void json_CreateNewFile(const char* filename);
void CreateEmptycJSONFile(const char* filePath); // #TODO Consider returning an error status

unsigned int GetObjectSize(const cJSON* cJSONObject);
unsigned int GetArraySize(const cJSON* cJSONArray);
