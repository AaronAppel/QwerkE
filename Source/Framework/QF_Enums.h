#pragma once

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

namespace QwerkE {

    QC_ENUM(eMaterialMaps, uint8_t,
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

    enum eOperationResult : uint8_t
    {
        Undefined = 0,
        Success,
        Failure
    };

    // #TODO Review for deprecation
    enum eComponentTags : uint8_t
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

    enum eRoutineTypes : uint8_t
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
    enum eLightType : uint8_t
    {
        LightType_Point = 0,
        LightType_Area,
        LightType_Spot,

        LightType_Max,
        LightType_NULL
    };

}
