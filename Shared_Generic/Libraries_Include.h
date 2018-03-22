#ifndef _Libraries_Include_H_
#define _Libraries_Include_H_

// TODO: Add Boost, Bullet3D

//////////////////////////////
// FILES //
//////////////////////////////
// JSON
// #include "Libraries/cJSON_Interface/cJSONInterface.h"

// images
#include "Libraries/lodepng/lodepng.h"
#include "stb_image.h"

// font
#include "Libraries/freetype2/ft2build.h"
#include FT_FREETYPE_H
//////////////////////////////

//////////////////////////////
// HID//
// xbox controller
#include "Libraries/EasyXinput/EasyXinput.h"
//////////////////////////////

//////////////////////////////
// Graphics //
// window creation
#include "Libraries/glew/GL/glew.h"

// openGL extensions
#include "Libraries/glfw/GLFW/glfw3.h"
//////////////////////////////

//////////////////////////////
// GUIs //
// imgui
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_impl_glfw_gl3.h"
//////////////////////////////

//////////////////////////////
// threading
#define HAVE_STRUCT_TIMESPEC // really?
#include "Libraries/pThreads/pthread.h"
//////////////////////////////

//////////////////////////////
// networking
// http://www.raknet.net/raknet/manual/tutorialsample3.html
//#include "Libraries/RakNet/Source/RakPeerInterface.h"
//////////////////////////////

//////////////////////////////
// math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/////////////////////////////

#endif // !_Libraries_Include_H_
