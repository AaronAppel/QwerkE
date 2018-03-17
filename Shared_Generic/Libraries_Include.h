#ifndef _Libraries_Include_H_
#define _Libraries_Include_H_

// include all libraries in the "Libraries" folder //

// HID //
#include "Libraries/EasyXinput/EasyXinput.h"

// FILES //
// JSON parsing
// #include "cJSON.h"
#include "Libraries/cJSON_Interface/cJSONInterface.h"

// image loading
#include "Libraries/lodepng/lodepng.h"
// TODO: stb_image

// font rendering
#include "Libraries/freetype2/ft2build.h"
#include FT_FREETYPE_H

// graphics //
#include "Libraries/glew/GL/glew.h"
#include "Libraries/glfw/GLFW/glfw3.h"

// GUIs //
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_impl_glfw_gl3.h"

#endif // !_Libraries_Include_H_
