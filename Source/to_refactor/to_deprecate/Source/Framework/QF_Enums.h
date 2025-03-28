#pragma once

#include <cstdint>

#include "Libraries/enum/QC_enum.h"

namespace QwerkE {

    enum class eInputDeviceTypes : std::uint8_t
    {
        // #TODO Deprecate or move to input files domain. Maybe a new input file like eKeys but eInputDevices
        Keyboard_Min = 0,
        Keyboard_Device0,
        Keyboard_Device1,
        Keyboard_Device2,
        Keyboard_Device3,
        Keyboard_Max,

        Mouse_Min,
        Mouse_Device0,
        Mouse_Device1,
        Mouse_Device2,
        Mouse_Device3,
        Mouse_Max,

        InputDeviceTypes_Max
    };

    QC_ENUM(eMaterialMaps, std::uint8_t,
        MatMap_Ambient = 0,
        MatMap_Diffuse,
        MatMap_Specular,
        MatMap_Emissive,
        MatMap_Height,
        MatMap_Normals,
        MatMap_Shininess,
        MatMap_Opacity,
        MatMap_Displacement,
        MatMap_LightMap,
        MatMap_Reflection,
        MatMap_Albedo,
        MatMap_Metallic,
        MatMap_AmbientOcclusion,
        MatMap_Roughness,

        MatMap_Vec3Colour,
        MatMap_Max,
        MatMap_Null
    );

    // enum class eMaterialMaps : std::uint8_t
    // {
    //     MatMap_Ambient = 0,
    //     MatMap_Diffuse,
    //     MatMap_Specular,
    //     MatMap_Emissive,
    //     MatMap_Height,
    //     MatMap_Normals,
    //     MatMap_Shininess,
    //     MatMap_Opacity,
    //     MatMap_Displacement,
    //     MatMap_LightMap,
    //     MatMap_Reflection,
    //     MatMap_Albedo,
    //     MatMap_Metallic,
    //     MatMap_AmbientOcclusion,
    //     MatMap_Roughness,
    //
    //     MatMap_Vec3Colour,
    //     MatMap_Max,
    //     MatMap_Null
    // };

    enum eOperationResult : std::uint8_t
    {
        Undefined = 0,
        Success,
        Failure
    };

    enum eComponentTags : std::uint8_t
    {
        Component_Null = 0,
        Component_Camera,
        Component_Physics,
        Component_Light,
        Component_Controller,
        Component_Render,
        Component_SkyBox,
        Component_SoundPlayer,
        Component_SoundListener,
        Component_Max // #TODO Don't create values that shouldn't be serialized. Create a ENUM_SIZE() macro or something instead
    };

    enum eRoutineTypes : std::uint8_t
    {
        // #TODO Enforce null/default values being the 1st (0) value, in all enums. Document rule as well!
        Routine_Null = 0, // #NOTE Value saved in data so can mess things up if deleted
        Routine_Render,
        Routine_Physics,
        Routine_Unused1,
        Routine_Transform,
        Routine_Max,
    };

    // Light Types
    enum eLightType : std::uint8_t // TODO: Hide from program?
    {
        LightType_Point = 0,
        LightType_Area,
        LightType_Spot,

        LightType_Max,
        LightType_NULL
    };

    // TODO: Adding object tags ruins object saving and loading. Fix!
    enum eGameObjectTags : std::uint8_t
    {
        GO_Tag_Player = 0,
        // Cameras
        GO_Tag_Camera,
        // Lights
        GO_Tag_Light,
        // Shapes
        GO_Tag_Cube,
        GO_Tag_Plane,
        GO_Tag_Sphere,
        // Scenery
        GO_Tag_SkyBox,
        // Test
        GO_Tag_TestModel,

        GO_Tag_Max,
        GO_Tag_Null,
    };

}
