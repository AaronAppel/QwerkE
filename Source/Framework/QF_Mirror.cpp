
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

#include "QF_ConfigHelper.h"
#include "QF_eKeys.h"

namespace QwerkE
{
	typedef QwerkE::eKeys eKeys;
	MIRROR_TYPE(eKeys)

	MIRROR_CLASS_START(FrameworkData)
	MIRROR_CLASS_MEMBER(QuickLoad);
	MIRROR_CLASS_MEMBER(MaxConcurrentThreadCount);
	MIRROR_CLASS_END

	MIRROR_CLASS_START(Libraries)
	MIRROR_CLASS_MEMBER(Rendering)
	MIRROR_CLASS_MEMBER(Audio)
	MIRROR_CLASS_MEMBER(Physics)
	MIRROR_CLASS_MEMBER(Networking)
	MIRROR_CLASS_MEMBER(Window)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(ScenesData)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(SceneSettings)
	MIRROR_CLASS_MEMBER(MaxEnabledScenes)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(Systems)
	MIRROR_CLASS_MEMBER(AudioEnabled)
	MIRROR_CLASS_MEMBER(PhysicsEnabled)
	MIRROR_CLASS_MEMBER(NetworkingEnabled)
	MIRROR_CLASS_MEMBER(ConsoleOutputWindowEnabled)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(EngineSettings)
	MIRROR_CLASS_MEMBER(LimitFramerate)
	MIRROR_CLASS_MEMBER(MaxFramesPerSecond)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(Controls)
	MIRROR_CLASS_MEMBER(Camera_MoveForward)
	MIRROR_CLASS_MEMBER(Camera_MoveBackward)
	MIRROR_CLASS_MEMBER(Camera_MoveLeft)
	MIRROR_CLASS_MEMBER(Camera_MoveRight)
	MIRROR_CLASS_MEMBER(Camera_MoveUp)
	MIRROR_CLASS_MEMBER(Camera_MoveDown)
	MIRROR_CLASS_MEMBER(Camera_RotateLeft)
	MIRROR_CLASS_MEMBER(Camera_RotateRight)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(UserData)
	MIRROR_CLASS_MEMBER(controls)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(ConfigData)
	MIRROR_CLASS_MEMBER(frameworkData);
	MIRROR_CLASS_MEMBER(libraries);
	MIRROR_CLASS_MEMBER(scenesData);
	MIRROR_CLASS_MEMBER(sceneSettings);
	MIRROR_CLASS_MEMBER(systems);
	MIRROR_CLASS_MEMBER(engineSettings)
	MIRROR_CLASS_END

}
