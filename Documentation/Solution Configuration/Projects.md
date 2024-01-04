## Qwerk_Engine #TODO Rename to platform including "WindowsOS" or something
This acts as the project for Visual Studio to use for compilation and execution.
The source files should be minimal and only handle platform specific engine code that should not exist in Shared_Engine, like defines for architecture, OS, debugging options, and special file includes like platform and system files.

## Shared_Common
Hold types and utility functions that are generic or have the least dependencies possible.
Ideally these source files can be copy and pasted into any other project to be instantly useful through application generic implementation.
## Shared_Engine
Hold engine specific code, like editor functionality, compilation, and more logic that should not be in or is built "on top of" the framework.
## Shared_Framework
For all systems and logic that are core to the application. This includes the scene graph, resource management, file loading and saving, game systems (audio, networking, physics, etc), and more. This logic is closer to the game than the engine logic so it exists at a "lower level" than the engine. Ideally, the engine and framework implementations allow editor only logic to exist in the engine project, and this logic can be stripped out of a game.exe with a single value change (preprocessor directive, file data, CLI argument, etc).