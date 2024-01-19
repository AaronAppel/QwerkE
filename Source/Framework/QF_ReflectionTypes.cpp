#include "QC_Reflection.h"

#include "QF_ConfigHelper.h"

// #TODO Look at reducing rigidity, and improving readability.
// Would it be cleaner to move defines under the class definition?
// Probably won't work in a header, unless inside the class.
// At least split up sections and communicate setup requirements nicely.

namespace QwerkE
{
    namespace Reflection // Type defines
    {
        typedef bool Bool; // For allowing basic type support in the TypeName enum
        typedef char Char;
        typedef int  Int;
        typedef char* CharPtr;

        typedef std::string String;
        typedef const char* ConstCharPtr;

        DEFINE_TYPE(int8_t)
        DEFINE_TYPE(int16_t)
        DEFINE_TYPE(int32_t)
        DEFINE_TYPE(uint8_t)
        DEFINE_TYPE(uint16_t)
        DEFINE_TYPE(uint32_t)

        DEFINE_TYPE(String)
        DEFINE_TYPE(ConstCharPtr)

        // DEFINE_TYPE(uint) // #TODO Fix "already defined" C2766, or maybe already supported by types above
        DEFINE_TYPE(Bool) // #TODO May require using special union types for serialization needs
        DEFINE_TYPE(Char)
        DEFINE_TYPE(CharPtr)

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