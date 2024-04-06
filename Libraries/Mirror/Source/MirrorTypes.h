#pragma once

// #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values (0,1...etc) in code
enum class MirrorTypes
{
    m_Invalid = 0,

    m_CLASSES_START,

    // ImGui
    ImGuiStyle,
    ImVec2,
    ImVec4,

    // Settings
    EngineSettings,
    RendererSettings,
    UserSettings,

    Project,

    // Types
    vec2f,
    vec3f,

    // Game
    FirstPersonCameraComponent,
    RenderComponent,
    FreeCameraComponent,
    StaticCameraComponent,
    ThirdPersonCameraComponent,
    RenderRoutine,
    LightComponent,
    Scene,
    SceneViewerData,
    Transform,
    Renderable,
    Routine,
    Component,
    Texture,
    TransformRoutine,
    ShaderProgram,
    Material,

    // Components
    ComponentCamera,
    ComponentMesh,
    ComponentScript,
    ComponentTransform,

    //
    EntityHandle,

    // External libraries
    m_enTT_Entity,
    m_entt_registry,
    m_CLASSES_END,

    m_ARRAYS_START,
    m_imvec4_array,
    m_floatArray16,
    m_ARRAYS_END,

    m_VECTORS_START,
    m_vector_eComponentTags,
    m_vector_entt_entities,
    m_vector_string,
    m_vector_routinePtr,
    m_vector_renderable,
    m_VECTORS_END,

    m_MAPS_START,
    m_MAPS_END,

    m_POINTERS_START, // #TODO Deprecate using std::is_pointer
    m_routinePtr,
    m_renderRoutinePtr,
    m_transformRoutinePtr,
    m_POINTERS_END,

    // Primitive types. #NOTE A prefix is used to avoid naming collisions with standard type keywords
    m_PRIMITIVES_START,

    m_ENUMS_START,
    eKeys,
    eSceneStates, // #TODO Review deprecating
    m_eSceneTypes,
    eComponentTags,
    m_ENUMS_END,

    m_PRIMITIVE_TYPES,
#include "MirrorEnumPrimitives.txt"
    m_PRIMITIVES_END,
};
