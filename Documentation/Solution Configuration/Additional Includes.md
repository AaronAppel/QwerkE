#TODO Make a list of additional includes, or at least a link to where they can be found
Maybe symlink a project file and link to the additional includes section. This would be to provide up to date includes without need to manual updating.

Examples :
- Library files
- Global header files
- Utilities


## #TODO  Major Issue
Anyone pulling down the project without a Visual Studio .sln (like on another platform or IDE), will encounter errors when files can't find each other due to missing project additional includes. This can be solved using project generation programs like premake and cmake. Something to explore more in the future, especially with more complex project configurations and need for build scripts execution.