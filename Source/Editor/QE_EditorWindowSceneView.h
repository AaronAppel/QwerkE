#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#ifdef _QBGFX
#include "Libraries/bgfx/include/bgfx/bgfx.h" // #TODO Abstract out bgfx from QE domain
#endif

#include "QF_ComponentCamera.h"
#include "QF_ComponentTransform.h"
#include "QF_FrameBuffer.h"
#include "QF_InputMouseDragTracker.h"
#include "QF_Texture.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	class Scene;
	class Shader;

	namespace Editor {

		class EditorWindowSceneView : public EditorWindow
		{
		public:
			// EditorWindowSceneView(u8 textureId, u8 viewId);

			EditorWindowSceneView(GUID guid = GUID());

			// void SetTextureId(u8 textureId) { m_TextureId = textureId; }
			// void SetViewId(u8 viewId) { m_ViewId = viewId; }

		private:
			void Init();
			void PickingSetup();

			void DrawInternal();
			void PickingUpdate();

			void PickingShutdown();

			void EditorCameraUpdate();

			void OnEditorWindowFocused(const EditorWindow* const a_FocusedWindow) override
			{
				if (a_FocusedWindow && Type() == a_FocusedWindow->Type())
				{
					m_IsLastFocusedSceneView = this != a_FocusedWindow;
				}
			}

			void OnEntitySelected(EntityHandle& entity) override
			{
				if (const Scene* entityScene = entity.GetScene(); entityScene == m_CurrentScene)
				{
					m_LastSelectEntity = entity;
				}
			}

			bool UsingEditorCamera()
			{
				return 0 == m_CurrentCameraComboIndex;
			}

			MIRROR_PRIVATE_MEMBERS

			// void OnSceneReload(const Scene* a_ReloadedScene) override { m_CurrentScene = nullptr; } // #TODO Review if needed

			bool m_IsLastFocusedSceneView = false;

			bool m_MouseStartedDraggingOnImage = false;

			EntityHandle m_LastSelectEntity = EntityHandle::InvalidHandle();

			s32 m_CurrentCameraComboIndex = 0;

			ComponentCamera m_EditorCamera;
			ComponentTransform m_EditorCameraTransform;
			Input::MouseDragTracker m_MouseDragTracker = Input::MouseDragTracker(QKey::e_MouseRight);

			Scene* m_CurrentScene = nullptr;
			GUID m_CurrentSceneGuid = GUID::Invalid;

			bool m_RenderingScene = true; // #TODO Consider serializing if useful

			char m_ScenesComboLabelBuffer[33] = "Scenes:    ##0000000000000000000";
			s32 m_LastSceneIndex = 0;

			// #TODO Scene transition changes
			// GUID m_SelectedSceneGuid;

			u16 m_ViewId = 0;

			Texture m_FrameBufferTextures[2];
			FrameBuffer m_FrameBuffer;

			// Picking
			bgfx::ViewId m_ViewIdShadingPass = 8; // Default forward rendered geo with simple shading
			bgfx::ViewId m_ViewIdIdPass = 9; // ID buffer for picking
			bgfx::ViewId m_ViewIdBlitPass = 10; // Blit GPU render target to CPU texture

#define ID_DIM 8  // Size of the ID buffer

			Shader* m_shadingProgram				= nullptr;
			Shader* m_idProgram						= nullptr;
			bgfx::UniformHandle u_tint				= BGFX_INVALID_HANDLE;
			bgfx::UniformHandle u_id				= BGFX_INVALID_HANDLE;
			bgfx::TextureHandle m_pickingRT			= BGFX_INVALID_HANDLE;
			bgfx::TextureHandle m_pickingRTDepth	= BGFX_INVALID_HANDLE;
			bgfx::TextureHandle m_blitTex			= BGFX_INVALID_HANDLE;
			bgfx::FrameBufferHandle m_pickingFB		= BGFX_INVALID_HANDLE;

			uint8_t m_blitData[ID_DIM * ID_DIM * 4]; // Read blit into this

			static const u8 m_Count = 12;
			float m_idsF[m_Count][4];
			uint32_t m_idsU[m_Count];

			uint32_t m_highlighted;

			uint32_t m_reading;
			uint32_t m_currFrame;
		};

	}

}