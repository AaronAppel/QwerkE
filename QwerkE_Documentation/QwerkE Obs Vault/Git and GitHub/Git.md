Remove a submodule :
1. git submodule deinit <submodule_directory>
2. git rm <submodule_directory>
3. rm -rf .git/modules/<submodule_directory>
4. commit changes