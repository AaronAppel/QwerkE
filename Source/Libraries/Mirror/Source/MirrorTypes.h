#pragma once

// #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values (0,1...etc) in code
enum class MirrorTypes
{
    // #TODO Deprecate categories like m_PAIRS_START. Not needed anymore
    m_Invalid = 0,

    TestStruct, // #NOTE For use with MirrorTesting.h
    Base, // #NOTE For use with MirrorTesting.h
    Derived1, // #NOTE For use with MirrorTesting.h
    Derived2, // #NOTE For use with MirrorTesting.h

    // BGFX Framework
    BgfxMesh,

    // ImGui
    ImGuiStyle,
    ImVec2,
    ImVec4,

    // Settings
    EngineSettings,
    RendererSettings,
    UserSettings,
    GameActions,

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
    EditorWindowPrefabScene,
    EditorWindowPrompt,

    EditorLastOpenedData,

    ProjectsData,

    ScriptGuiButton,
    CallBackFunction,

    // External libraries
    m_enTT_Entity,
    m_entt_registry,

    m_pair_mirrorTypes_vec_pair_guid_string,
    m_pair_guid_string,
    m_pair_guid_stringPtr,
    m_pair_guid_voidPtr,
    m_pair_eScriptTypes_ScriptablePtr,
    m_pair_guid_enttEntity,
    m_pair_guid_editorWindowPtr, // Editor only
    m_pair_string_int32, // #NOTE For use with MirrorTesting.h

    m_imvec4_array,
    m_arr_float16,
    m_arr_float10, // #NOTE For use with MirrorTesting.h

    m_vec_pair_guid_string,
    m_vec_string,
    m_vector_entt_entities,
    m_vec_char, // #NOTE For use with MirrorTesting.h

    m_map_mirrorTypes_vec_pair_guid_string, // #TODO Review umaps vs non-umap
    m_map_guid_voidPtr,
    m_map_guid_stringPtr,
    m_map_eScriptTypes_ScriptablePtr,
    m_map_guid_entt,
    m_umap_guid_editorWindowPtr,
    m_umap_string_int32, // #NOTE For use with MirrorTesting.h

    m_scriptablePtr,
    m_editorWindowPtr, // Editor only
    m_int32Ptr, // #NOTE For use with MirrorTesting.h
    m_derived1Ptr, // #NOTE For use with MirrorTesting.h
    m_derived2Ptr, // #NOTE For use with MirrorTesting.h
    m_basePtr, // #NOTE For use with MirrorTesting.h
    m_vec_string_ptr,

    eKeys,
    eSceneStates, // #TODO Review deprecating
    eScriptTypes,
    m_eSceneTypes,
    eComponentTags,
    EditorWindowTypes, // Editor only
    EditorWindowFlags, // Editor only

    MirrorTypes,
    // #NOTE A prefix is used to avoid naming collisions with standard type keywords
    m_int8_t,
    m_int16_t,
    m_int32_t,
    m_int64_t,

    m_uint8_t,
    m_uint16_t,
    m_uint32_t,
    m_uint64_t,

    m_bool,
    m_char,
    m_float,
    m_double,
    m_charPtr,
    m_constCharPtr,

    m_string,
    m_stringPtr,

    m_void,
    m_voidPtr,
};
