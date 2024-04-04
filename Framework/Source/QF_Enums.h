#pragma once

#include <cstdint>

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

namespace QwerkE {

    QC_ENUM(eMaterialMaps, u8,
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

    enum eOperationResult : u8
    {
        Undefined = 0,
        Success,
        Failure
    };

    enum eComponentTags : u8
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

    enum eRoutineTypes : u8
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
    enum eLightType : u8
    {
        LightType_Point = 0,
        LightType_Area,
        LightType_Spot,

        LightType_Max,
        LightType_NULL
    };

    // TODO: Adding object tags ruins object saving and loading. Fix!
    enum eGameObjectTags : u8
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
