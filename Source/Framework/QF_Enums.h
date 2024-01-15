#pragma once

#include <cstdint>

namespace QwerkE
{

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

        GamePad_Min,
        GamePad_Device0,
        GamePad_Device1,
        GamePad_Device2,
        GamePad_Device3,
        GamePad_Device4,
        GamePad_Device5,
        GamePad_Device6,
        GamePad_Device7,
        GamePad_Max,

        Max_Device
    };

    enum class eMaterialMaps : std::uint8_t
    {
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
    };

    // #TODO Move engine enums to their own engine specific file
    enum class eEngineMessage : std::uint8_t
    {
        _QSuccess = 0,
        _QFailure
    };

    enum class eEngineServices : std::uint16_t // TODO: Rename engine to framework
    {
        // Resource_Manager = 0,
        // Input_Manager,
        // Audio_Manager,
        // Data_Manager,
        // Graphics (OpenGLManager)
        // Event_System,
        // Scene_Manager,
        // Factory_Entity,
        // Factory_Mesh,
        // Factory_Shader,
        // Physics,
        // MessageManager,
        // Renderer,
        // Jobs,
        // Windows,
        // Network,
        // FileSystem,

        Service_Max
    };

    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum eCamera_Movement : std::uint8_t
    {
        FORWARD = 0,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        RROTATE,
        LROTATE
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
        Routine_Null = 0,
        Routine_Render,
        Routine_Physics,
        Routine_Print,
        Routine_Transform,
        Routine_Max,
    };

    /* Scenes */
    // #TODO Deprecate as scenes should exist in data and be referenced by file names
    enum eSceneTypes : std::uint8_t
    {
        Scene_Null = 0,
        Scene_GameScene,
        Scene_TestScene,

        Scene_ViewerScene,
        Scene_PBR_Test1,

        Scene_Max,
    };

    // Camera Types
    enum eCamType : std::uint8_t // TODO: Hide from program?
    {
        CamType_FreeCam = 0,
        CamType_FirstPerson,
        CamType_ThirdPerson,
        CamType_Static,

        CamType_Max,
        CamType_NULL
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
