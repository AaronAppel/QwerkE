#ifndef __DebugHelpers_H_
#define __DebugHelpers_H_

// DEBUG:: This file is meant aid development by
// offering utility features such as global input access.
// This file is not meant to be used in published code
// and should be removed from release versions and builds.
class Controller;
Controller* debug_GetPlayerController();

bool debug_IsKeyDown(int GLFWkey);
void debug_GetCurrentWindowSize(int* x, int* y);

class GameCore;
GameCore* debug_GetGameCore();

double debug_GetDeltaTime();

#endif // __DebugHelpers_H_