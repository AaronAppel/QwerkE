#pragma once

// #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values (0,1...etc) in code
enum class MirrorTypes
{
    m_Invalid = 0,

    m_CLASSES_START,

    TestStruct, // #NOTE For use with MirrorTesting.h

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

    // Components
    ComponentCamera,
    ComponentInfo,
    ComponentLight,
    ComponentMesh,
    ComponentScript,
    ComponentTransform,

    // Scripts
    Scriptable,
    ScriptableCamera,
    ScriptablePathFinder,
    ScriptablePatrol,
    ScriptableTesting,

    // Misc
    Scene,
    EntityHandle,
    GUID,
    Mesh,
    Shader,

    // Editor types
    EditorWindow,
    EditorWindowAssets,
    EditorWindowDefaultDebug,
    EditorWindowDockingContext,
    EditorWindowEntityInspector,
    EditorWindowImGuiDemo,
    EditorWindowMenuBar,
    EditorWindowSceneControls,
    EditorWindowSceneGraph,
    EditorWindowSceneView,
    EditorWindowSettings,
    EditorWindowStylePicker,
    EditorWindowMaterialEditor,
    EditorWindowFolderViewer,
    EditorWindowNodeEditor,
    EditorWindowShaderEditor,

    ScriptGuiButton,
    CallBackFunction,

    // External libraries
    m_enTT_Entity,
    m_entt_registry,

    m_CLASSES_END,

    m_PAIRS_START,
    m_pair_guid_string,
    m_pair_eScriptTypes_ScriptablePtr,
    m_pair_guid_enttEntity,
    m_pair_guid_editorWindowPtr, // Editor only
    m_pair_string_int32, // #NOTE For use with MirrorTesting.h
    m_PAIRS_END,

    m_ARRAYS_START,
    m_imvec4_array,
    m_arr_float16,
    m_arr_float10, // #NOTE For use with MirrorTesting.h
    m_ARRAYS_END,

    m_VECTORS_START,
    m_vec_pair_guid_string,
    m_vec_string,
    m_vector_entt_entities,
    m_vec_char, // #NOTE For use with MirrorTesting.h
    m_VECTORS_END,

    m_MAPS_START,
    m_map_eScriptTypes_ScriptablePtr,
    m_map_guid_entt,
    m_umap_guid_editorWindowPtr,
    m_umap_string_int32, // #NOTE For use with MirrorTesting.h
    m_MAPS_END,

    m_POINTERS_START, // #TODO Deprecate using std::is_pointer
    m_scriptablePtr,
    m_editorWindowPtr, // Editor only
    m_int32Ptr, // #NOTE For use with MirrorTesting.h
    m_POINTERS_END,

    // Primitive types. #NOTE A prefix is used to avoid naming collisions with standard type keywords
    m_PRIMITIVES_START,

    m_ENUMS_START,
    eKeys,
    eSceneStates, // #TODO Review deprecating
    eScriptTypes,
    m_eSceneTypes,
    eComponentTags,
    EditorWindowTypes, // Editor only
    EditorWindowFlags, // Editor only
    m_ENUMS_END,

    m_PRIMITIVE_TYPES,
#include "MirrorEnumPrimitives.txt"
    m_PRIMITIVES_END,
};
