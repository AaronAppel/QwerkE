
#include <vector>
#include <string>

#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

#include "QF_eKeys.h"
#include "QF_Settings.h"

namespace QwerkE
{

	typedef std::vector<std::string> m_vector_string;
	MIRROR_TYPE(m_vector_string)

	typedef QwerkE::eKeys eKeys;
	MIRROR_TYPE(eKeys)

	MIRROR_CLASS_START(EngineSettings)
	MIRROR_CLASS_MEMBER(windowWidthPixels)
	MIRROR_CLASS_MEMBER(windowHeightPixels)
	MIRROR_CLASS_MEMBER(showFps)
	MIRROR_CLASS_MEMBER(limitFramerate)
	MIRROR_CLASS_MEMBER(maxFramesPerSecond)
	MIRROR_CLASS_MEMBER(maxEnabledScenes)
	MIRROR_CLASS_MEMBER(maxConcurrentThreadCount)
	MIRROR_CLASS_MEMBER(audioEnabled)
	MIRROR_CLASS_MEMBER(physicsEnabled)
	MIRROR_CLASS_MEMBER(networkingEnabled)
	MIRROR_CLASS_MEMBER(consoleOutputWindowEnabled)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(UserSettings)
	MIRROR_CLASS_MEMBER(key_camera_MoveForward)
	MIRROR_CLASS_MEMBER(key_camera_MoveBackward)
	MIRROR_CLASS_MEMBER(key_camera_MoveLeft)
	MIRROR_CLASS_MEMBER(key_camera_MoveRight)
	MIRROR_CLASS_MEMBER(key_camera_MoveUp)
	MIRROR_CLASS_MEMBER(key_camera_MoveDown)
	MIRROR_CLASS_MEMBER(key_camera_RotateLeft)
	MIRROR_CLASS_MEMBER(key_camera_RotateRight)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(ProjectSettings)
	MIRROR_CLASS_MEMBER(projectName)
	MIRROR_CLASS_MEMBER(assetsDirPath)
	MIRROR_CLASS_MEMBER(sceneFileNames)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(SceneViewerData)
	MIRROR_CLASS_MEMBER(maxEnabledScenes)
	MIRROR_CLASS_MEMBER(maxDisplayedScenes)
	MIRROR_CLASS_MEMBER(viewDistance)
	MIRROR_CLASS_END

}
