March 31st, 2025
Changing scene loading to hopefully stream line the process.
#TODO Create a "How To" doc for scene creation, deletion, loading, unloading, and editing.

The Assets class is creating everything but scenes currently. That does create some inconsistent logic around the editor. If a new scene is needed, a new file must first be created. Scenes should not exist in RAM unless the file has been created first.

The Assets manager should load any asset type, but not hold the implementation. Instead, calls to loading functions like Scenes::CreateSceneFromFile() should be used, whenever possible/appropriate. The Scenes namespace currently holds the RAM for scenes, but the Assets class loads and unloads as it can more easily track the registry this way, un-intrusively.