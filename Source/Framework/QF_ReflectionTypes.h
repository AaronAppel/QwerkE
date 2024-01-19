#pragma once

// Add type names here to support reflection/serialization (case sensitive)

enum class ReflectionType { // #TODO It would be nice to wrap in a namespace

    // #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values in code

    // User defined types
    ConfigData,
    Controls,
    eKeys,
    EngineSettings,
    FrameworkData,
    Libraries,
    ScenesData,
    SceneSettings,
    Systems,
    UserData,

    // Primitive types. #NOTE A prefix is used to avoid naming collisions with standard type keywords
    r_int8_t,
    r_int16_t,
    r_int32_t,
    r_int64_t,

    r_uint8_t,
    r_uint16_t,
    r_uint32_t,
    r_uint64_t,

    r_int,
    r_bool,
    r_char,
    r_charPtr,
    r_float,
    r_double,
    r_string,
    r_constCharPtr
};
