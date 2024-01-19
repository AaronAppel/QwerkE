#include "QC_Reflection.h"

#include "QF_ConfigHelper.h"

#include <string>

// #TODO Look at reducing rigidity, and improving readability.
// Would it be cleaner to move defines under the class definition?
// Probably won't work in a header, unless inside the class.
// At least split up sections and communicate setup requirements nicely.

namespace QwerkE
{
    namespace Reflection // Type defines
    {
        typedef int8_t r_int8_t;
        typedef int16_t r_int16_t;
        typedef int32_t r_int32_t;
        typedef int64_t r_int64_t;
        typedef uint8_t r_uint8_t;
        typedef uint16_t r_uint16_t;
        typedef uint32_t r_uint32_t;
        typedef uint64_t r_uint64_t;

        DEFINE_TYPE(r_int8_t)
        DEFINE_TYPE(r_int16_t)
        DEFINE_TYPE(r_int32_t)
        DEFINE_TYPE(r_int64_t)
        DEFINE_TYPE(r_uint8_t)
        DEFINE_TYPE(r_uint16_t)
        DEFINE_TYPE(r_uint32_t)
        DEFINE_TYPE(r_uint64_t)

        typedef bool r_bool; // For allowing basic type support in the ReflectionType enum
        typedef char r_char;
        typedef char* r_charPtr;

        DEFINE_TYPE(r_bool) // #TODO May require using special union types for serialization needs
        DEFINE_TYPE(r_char)
        DEFINE_TYPE(r_charPtr)

        typedef std::string r_string;
        typedef const char* r_constCharPtr;

        DEFINE_TYPE(r_string)
        DEFINE_TYPE(r_constCharPtr)

        typedef QwerkE::eKeys eKeys;
        DEFINE_TYPE(eKeys)

        DEFINE_TYPE(Controls)
        DEFINE_TYPE(UserData)

        DEFINE_TYPE(FrameworkData)
        DEFINE_TYPE(Libraries)
        DEFINE_TYPE(ScenesData)
        DEFINE_TYPE(SceneSettings)
        DEFINE_TYPE(Systems)
        DEFINE_TYPE(EngineSettings)
        DEFINE_TYPE(ConfigData)
    }

    namespace Reflection // Class and member attributes
    {
        BEGIN_ATTRIBUTES_FOR(FrameworkData)
        DEFINE_MEMBER(QuickLoad);
        DEFINE_MEMBER(MaxConcurrentThreadCount);
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(Libraries)
        DEFINE_MEMBER(Rendering)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(ScenesData)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(SceneSettings)
        DEFINE_MEMBER(MaxEnabledScenes)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(Systems)
        DEFINE_MEMBER(AudioEnabled)
        DEFINE_MEMBER(PhysicsEnabled)
        DEFINE_MEMBER(NetworkingEnabled)
        DEFINE_MEMBER(ConsoleOutputWindowEnabled)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(EngineSettings)
        DEFINE_MEMBER(LimitFramerate)
        DEFINE_MEMBER(MaxFramesPerSecond)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(Controls)
        DEFINE_MEMBER(Camera_MoveForward)
        DEFINE_MEMBER(Camera_MoveBackward)
        DEFINE_MEMBER(Camera_MoveLeft)
        DEFINE_MEMBER(Camera_MoveRight)
        DEFINE_MEMBER(Camera_MoveUp)
        DEFINE_MEMBER(Camera_MoveDown)
        DEFINE_MEMBER(Camera_RotateLeft)
        DEFINE_MEMBER(Camera_RotateRight)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(UserData)
        DEFINE_MEMBER(controls)
        END_ATTRIBUTES

        BEGIN_ATTRIBUTES_FOR(ConfigData)
        DEFINE_MEMBER(framework);
        DEFINE_MEMBER(libraries);
        DEFINE_MEMBER(scenesData);
        DEFINE_MEMBER(sceneSettings);
        DEFINE_MEMBER(systems);
        DEFINE_MEMBER(engineSettings)
        END_ATTRIBUTES
    }

}