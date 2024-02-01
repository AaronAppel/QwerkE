#pragma once

// Define libraries and systems to build
#define GLFW3
#define GLEW
#define OpenGL
#define OpenAL
#define dearimgui
#define BULLET3
#define RAKNET
#define LODEPNG

// MSVC pre-preocessor defines : https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170

#ifdef _DEBUG
#define _QDebug
#define QWERKE_TRACKING_ALLOCATIONS 0
#else
#define QWERKE_TRACKING_ALLOCATIONS 0
#define _QRelease
#endif

// Define API level for various functionality
// Currently not being used.
#define _QTesting // Enable testing features and suites

#define _QExample // Working example to show functionality

#ifndef _QAdvancedApi // Fully unlocked API
#define _QSimpleApi // Limited API for simpler usability off the start
#endif

#ifndef EngineName
#define ProjectName "Qwerk"
#endif

#ifndef AssetsDir
#define AssetsDir "Assets/"
#endif

#ifndef LibrariesDir
#define LibrariesDir "Libraries/"
#endif

#include "QC_StringHelpers.h"

//USER: Set the offset from your project working directory
// You must #define AssetDir "Assets/" (path to assets)

#define FrameworkNullAssetsDir StringAppend(AssetsDir, "NullAssets/") // Framework null objects to use if user assets fail to load

// Define "hard coded" file names for handling null objects for ease of use
// but also to ensure consistency and prevent unintended changes.
// Any external files should have matching names for the same reasons.
// TODO: should macros express the file extension? Need to if the loader is looking for that file
// TODO: Procedurally generate null object to guarantee instantiation
// TODO: Use file extension as name consistently. Either everywhere, or no where
#define NullFolderPath(fullFileName) StringAppend(FrameworkNullAssetsDir, fullFileName) // TODO: What is this for?

// Define paths to resource folders that can change easily from project
// to project using a preprocessor define. Paths can also change or be
// manipulated at runtime so be careful since this is a good and bad thing.
// #define MeshFolder AssetDir "Meshes/"

#define MeshesFolderPath(fullFileName) StringAppend(AssetsDir, "Meshes/", fullFileName)
#define ShadersFolderPath(fullFileName) StringAppend(AssetsDir, "Shaders/", fullFileName)
#define TexturesFolderPath(fullFileName) StringAppend(AssetsDir, "Textures/", fullFileName)
#define FontsFolderPath(fullFileName) StringAppend(AssetsDir, "Fonts/", fullFileName)
#define SoundsFolderPath(fullFileName) StringAppend(AssetsDir, "Sounds/", fullFileName)
#define ScenesFolderPath(fullFileName) StringAppend(AssetsDir, "Scenes/", fullFileName)
#define ConfigsFolderPath(fullFileName) StringAppend(AssetsDir, "Configs/", fullFileName)
#define PreferencesFolderPath(fullFileName) StringAppend(AssetsDir, "Configs/", fullFileName)
#define ObjectSchematicsFolderPath(fullFileName) StringAppend(AssetsDir, "BluePrints_Prefabs_Schematic/", fullFileName)
#define ProjectsFolderPath(fullFileName) StringAppend(AssetsDir, "Projects/", fullFileName)

// Standardized file extensions
#define vertex_shader_ext "vert" // #TODO Should extensions be implied to own the '.' period?
#define fragment_shader_ext "frag"
#define geometry_shader_ext "geo"
#define object_schematic_ext "osch"
#define material_schematic_ext "msch"
#define shader_schematic_ext "ssch"
#define scene_ext "qscene"
#define configs_ext "qconf"
#define preferences_ext "qpref"
#define projects_ext "qproj"

// Define "hard coded" file names for handling null objects for ease of use
// but also to ensure consistency and prevent unintended changes.
// TODO: should macros express the file extension? Need to if the loader is looking for that file
// TODO: Use file extension as name consistently. Either everywhere, or no where (probably use them everywhere)
#define null_config StringAppend("null_config.", configs_ext)
#define null_font "null_font.ttf"
#define null_material StringAppend("null_material.", material_schematic_ext)
#define null_mesh "null_mesh"
#define null_mesh_filename "null_mesh.obj" // #TODO Look to deprecate
#define null_preferences StringAppend("null_preferences.", preferences_ext)
#define null_scene StringAppend("null_scene.", scene_ext)
#define null_shader "null_shader"
#define null_sound "null_sound.wav"
#define null_texture "null_texture.png"
#define null_project StringAppend("null_project.", projects_ext)

// Null schematics
#define null_material_schematic StringAppend("null_material.", material_schematic_ext)
#define null_object_schematic StringAppend("null_object.", object_schematic_ext)
#define null_shader_schematic StringAppend("null_shader.", shader_schematic_ext)
#define null_vert_component StringAppend("null_shader.", vertex_shader_ext)
#define null_frag_component StringAppend("null_shader.", fragment_shader_ext)
#define null_geo_component StringAppend("null_shader.", geometry_shader_ext)
