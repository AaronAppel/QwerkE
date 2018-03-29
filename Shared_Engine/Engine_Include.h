#ifndef _Engine_Include_H_
#define _Engine_Include_H_

#include "Engine_Defines.h" // platform, architecture,
#include "Engine_Platform.h" // platform libraries

// enums, constants

#include "../Shared_Generic/Libraries_Include.h" // openGL, Bullet, GLFW, etc

//////////////////////////////
// Infrastructure //
//////////////////////////////
// Graphics //
#include "Systems/Graphics_Header.h"

// Input //
#include "Systems/Input/Input.h"

// Scene //
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

// GameCore //
#include "Systems/GameCore.h"

#include "Systems/ServiceLocator.h" // Audio, Input, Graphics, etc
//////////////////////////////

#endif // !_Engine_Include_H_
