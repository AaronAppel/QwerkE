#pragma once

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
// #define NullFolderPath(fullFileName) StringAppend(FrameworkNullAssetsDir, fullFileName) // TODO: What is this for?

// Define paths to resource folders that can change easily from project
// to project using a preprocessor define. Paths can also change or be
// manipulated at runtime so be careful since this is a good and bad thing.
// #define MeshFolder AssetDir "Meshes/"

#define NullAssetsFolderPath(fullFileName) StringAppend(NullAssetsDir, fullFileName)

#define MeshesFolderPath(fullFileName) StringAppend(AssetsDir, "Meshes/", fullFileName)
#define ShadersFolderPath(fullFileName) StringAppend(AssetsDir, "Shaders/", fullFileName)
#define TexturesFolderPath(fullFileName) StringAppend(AssetsDir, "Textures/", fullFileName)
#define FontsFolderPath(fullFileName) StringAppend(AssetsDir, "Fonts/", fullFileName)
#define SoundsFolderPath(fullFileName) StringAppend(AssetsDir, "Sounds/", fullFileName)
#define ScenesFolderPath(fullFileName) StringAppend(AssetsDir, "Scenes/", fullFileName)
#define SettingsFolderPath(fullFileName) StringAppend(AssetsDir, "Settings/", fullFileName)
#define ProjectsFolderPath(fullFileName) StringAppend(AssetsDir, "Projects/", fullFileName)

#define SchematicsFolderPath(fullFileName) StringAppend(AssetsDir, "Schematics/", fullFileName)

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
#define projects_file_ext "qproj"
#define mesh_file_ext "obj"
#define font_file_ext "ttf"
#define sound_file_ext "wav"
#define texture_file_ext "png"
#define settings_file_ext "qsetting"

// Define "hard coded" file names for handling null objects for ease of use
// but also to ensure consistency and prevent unintended changes.
// TODO: should macros express the file extension? Need to if the loader is looking for that file
// TODO: Use file extension as name consistently. Either everywhere, or no where (probably use them everywhere)
#define null_config StringAppend("null_config.", configs_ext) // #TODO Make/find a constexpr string builder
#define null_font StringAppend("null_font.", font_file_ext)
#define null_material StringAppend("null_material.", material_schematic_ext)
#define null_mesh StringAppend("null_mesh.", mesh_file_ext)
#define null_preferences StringAppend("null_preferences.", preferences_ext)
#define null_scene StringAppend("null_scene.", scene_ext)
#define null_shader "null_shader"
#define null_sound StringAppend("null_sound.", sound_file_ext)
#define null_texture StringAppend("null_texture.", texture_file_ext)
#define null_project StringAppend("null_project.", projects_file_ext)
#define null_setting StringAppend("null_setting.", settings_file_ext)

#define null_material_schematic StringAppend("null_material.", material_schematic_ext)
#define null_object_schematic StringAppend("null_object.", object_schematic_ext)
#define null_shader_schematic StringAppend("null_shader.", shader_schematic_ext)

#define null_vert_component StringAppend("null_shader.", vertex_shader_ext)
#define null_frag_component StringAppend("null_shader.", fragment_shader_ext)
#define null_geo_component StringAppend("null_shader.", geometry_shader_ext)
