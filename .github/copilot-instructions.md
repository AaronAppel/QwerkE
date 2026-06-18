# Copilot instructions for QwerkE

This repository is a C++ hobby game engine. Keep guidance concise and focused on repository-specific workflows.

## Build, test, and lint commands
- Primary entry: run "START HERE.bat" in the repository root to generate and open the Visual Studio 2022 solution (recommended for Windows devs).
- Manual build (Visual Studio): open QwerkE.sln and Build -> Solution.
- Build scripts (CLI): Build\Build-QwerkE.lua and premake in Build/ are used for project generation. There are helper .bat files in Build/ (e.g., Setup-Windows-Vs2022.bat, CompileAllShaders.bat).
- Running a single test: this repo does not include an automated test suite. If tests are added, run them via the test runner configured (describe runner in this file).
- Linting: no repository-wide linter configured. Apply project or IDE-specific linters (clang-tidy, cpplint) if introduced.

## High-level architecture
- Language & platform: Native C++ targeting Windows (Visual Studio solutions present). Project uses premake to generate build files.
- Engine structure (big picture): core engine modules (rendering, scene/ecs, physics integration, resource loading) live alongside example/game projects. Third-party libs (bgfx, entt, Bullet, stb_image, imgui, freetype) are integrated as external dependencies under Build configuration.
- Asset pipeline: Shaders and meshes are built/compiled via scripts in Build/ (CompileAllShaders.bat, CompileAllMeshes.bat). Runtime assets and built artifacts appear under bin\windows-x86_64-<Configuration>.
- Typical dev workflow: run START HERE.bat -> open generated solution -> build in Visual Studio -> run game project executable from bin. Use Build scripts when regenerating projects or rebuilding assets.

## Key conventions and repository-specific patterns
- Build generation: premake + Lua scripts in Build/ — prefer modifying Build\Build-QwerkE.lua and Libraries.lua when changing project structure or third-party integrations.
- Windows-first tooling: many helper scripts are .bat; paths and assumptions target Windows/Visual Studio.
- Shaders/Assets: compiled via Build scripts — do not manually edit compiled shader blobs in bin; edit source shader files in the source tree and re-run CompileAllShaders.bat.
- External libs: referenced in Build/Libraries.lua — updating or adding a library should be done through that file so premake integrates it correctly.
- No centralized test or CI config: expect local, manual testing via running executables.

## Files to consult when automating tasks
- README.md — higher-level project notes and links
- Build/Build-QwerkE.lua and Build/Libraries.lua — project generation and dependency declarations
- START HERE.bat — canonical entry for new devs on Windows
- Build/CompileAllShaders.bat, Build/CompileAllMeshes.bat — asset build steps

## When updating these instructions
- If CI, tests, or a linter are added, update "Build, test, and lint commands" with exact commands and how to run a single test.
- If non-Windows support is introduced, add platform-specific build and run steps.

---

(Imported top-level notes from README.md where relevant.)
