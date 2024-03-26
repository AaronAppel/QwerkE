The Lumix engine : https://github.com/nem0/LumixEngine
Seems like an interesting project to check out. Unfortunately it isn't building locally because I installed Visual Studio 2022 on my A: drive, instead of the C: drive so the build scripts can't find devenv.exe or such.

The build system for the Lumix engine (projects/start/main.bat) seems pretty interesting on it's own. I see it downloads or clones libraries if they are not on the user's machine already. It also handles downloading libraries like PhysX, so maybe something to play around with in QwerkE in the future.