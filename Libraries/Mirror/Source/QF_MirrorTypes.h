#pragma once

// #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values (0,1...etc) in code
enum class MirrorTypes
{
    Null = 0,

    // User defined types
    // Add type names here to support Mirror reflection (case sensitive)

    // #NOTE Better in alphabetical order

    m_CLASSES_START,

    // Settings
    EngineSettings,
    RendererSettings,
    UserSettings,
    ProjectSettings,

    // Types
    vec2,
    vec3,

    // Game
    ComponentCamera,
    GameObject,
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

    // External libraries
    m_enTT_Entity,
    m_entt_registry,
    m_CLASSES_END,

    m_VECTORS_START,
    m_vector_eComponentTags,
    m_vector_entt_entities,
    m_vector_string,
    m_vector_gameobjectPtr,
    m_vector_routinePtr,
    m_vector_renderable,
    m_VECTORS_END,

    m_MAPS_START,
    m_map_eComponentTags_componentPtr,
    m_map_eMaterialMaps_texturePtr,
    m_MAPS_END,

    m_POINTERS_START, // #TODO Deprecate using std::is_pointer
    m_gameObjectPtr,
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
    eGameObjectTags,
    eComponentTags,
    m_ENUMS_END,

#include "MirrorEnumPrimitives.txt"
    m_PRIMITIVES_END,
};
