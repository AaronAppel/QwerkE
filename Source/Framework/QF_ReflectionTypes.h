#pragma once

// Add type names here to support reflection/serialization (case sensitive)
enum class TypeName {

	int8_t,
	int16_t,
	int32_t,

	uint8_t,
	uint16_t,
	uint32_t,

    FrameworkData,
    Libraries,
    ScenesData,
    SceneSettings,
    Systems,
    EngineSettings,
    Controls,

    UserData,
    ConfigData,

    uint,

    Int, // Primitives need upper casing, and a matching typedef to work
    Bool,
    Char,
    CharPtr,
    Float,
    Double,
    String,
    ConstCharPtr,

    eKeys
    // std::string,

    // long
};
