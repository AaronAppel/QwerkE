#include "QF_Debug.h"

#ifdef _QDEBUG

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/timer.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif // _QBGFXFRAMEWORK
#endif // _QBGFX

#include "QF_Renderer.h"

namespace QwerkE {

	namespace Debug {

		// #TODO Check out from bgfx example solution:
		// dde.setStipple(true, 1.0f, time * 0.1f);
		// dde.setSpin(time * 0.3f);
		// dde.setLod(0);
		// intersect(&dde, ray, aabb);
		// dde.setTransform(bunny);
		// dde.drawFrustum(mtxVp);
		// dde.push();
		// dde.pop();

		void DrawArc(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;
		}

		void DrawAxis(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;
		}

		void DrawCapsule(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;
		}

		void DrawCircle(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;
		}

		void DrawCone(uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

		void DrawCube(const uint16_t a_ViewId, const vec3f a_Position, const float a_Size, const bool a_Wireframe, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			bgfx::setState(BGFX_STATE_DEFAULT); // #TODO Remove?

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(a_ViewId, true);

			const float halfSize = a_Size * 0.5f;
			bx::Aabb aabb =
			{
				{  a_Position.x - halfSize, a_Position.y - halfSize, a_Position.z - halfSize },
				{  a_Position.x + halfSize, a_Position.y + halfSize, a_Position.z + halfSize }
			};
			debugDrawer.setWireframe(a_Wireframe);
			debugDrawer.setColor(a_Color);
			debugDrawer.draw(aabb);
			debugDrawer.end();
		}

		void DrawCylinder(uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

		void DrawFrustum(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

		void DrawGrid(const uint16_t a_ViewId, const vec3f a_Position, u16 a_Size, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			uint64_t bgfxState = 0
				| BGFX_STATE_DEPTH_TEST_MASK
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_CULL_CW
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				;
			bgfx::setState(bgfxState); // BGFX_STATE_DEFAULT

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(a_ViewId, true);

			constexpr bx::Vec3 normal = { .0f,  1.f, .0f };
			const bx::Vec3 pos = { a_Position.x, a_Position.y, a_Position.z };

			bx::Plane plane(bx::InitNone);
			bx::calcPlane(plane, normal, pos);

			debugDrawer.setColor(a_Color);
			debugDrawer.drawGrid(Axis::Y, pos, a_Size, 1.0f);
			debugDrawer.end();
		}

		void DrawLine(const uint16_t a_ViewId, const vec3f a_Start, const vec3f a_End, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			bgfx::setState(BGFX_STATE_DEFAULT); // #TODO Remove?

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(a_ViewId, true);

			const bx::Vec3 start = { a_Start.x, a_Start.y, a_Start.z };
			const bx::Vec3 end = { a_End.x, a_End.y, a_End.z };
			debugDrawer.setColor(a_Color);
			debugDrawer.drawLine(start, end);
			debugDrawer.end();
		}

		void DrawLineList(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;
		}

		void DrawOrb(uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

		void DrawQuad(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

		void DrawSphere(const uint16_t a_ViewId, const vec3f a_Position, const float a_Scale, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			bgfx::setState(BGFX_STATE_DEFAULT); // #TODO Remove?

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(a_ViewId, true);
			debugDrawer.setColor(a_Color);
			debugDrawer.drawSphere(a_Position.x, a_Position.y, a_Position.z, a_Scale, Axis::X);
			debugDrawer.end();
		}

		void DrawTriList(const uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;
		}

	}

}
#endif // _QDEBUG
