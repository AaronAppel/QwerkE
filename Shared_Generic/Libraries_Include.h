#ifndef _Libraries_Include_H_
#define _Libraries_Include_H_

//////////////////////////////
// FILES //
//////////////////////////////
// JSON
#include "Libraries/cJSON_Interface/cJSONInterface.h"
// images
#include "Libraries/lodepng/lodepng.h"
// stb_image

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
// pthreads
//////////////////////////////

//////////////////////////////
// networking
// http://www.raknet.net/raknet/manual/tutorialsample3.html
//#include "Libraries/RakNet/Source/RakPeerInterface.h"
//#include "Libraries/RakNet/Source/MessageIdentifiers.h"
//#include "Libraries/RakNet/Source/BitStream.h"
//#include "Libraries/RakNet/Source/RakNetTypes.h"  // MessageID
//////////////////////////////

#endif // !_Libraries_Include_H_
