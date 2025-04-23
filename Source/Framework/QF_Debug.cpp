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

		void DrawLine(const vec3f a_Start, const vec3f a_End, uint32_t a_Color)
		{
			// #TODO Check out from bgfx example solution:
			// dde.setStipple(true, 1.0f, time * 0.1f);
			// dde.setSpin(time * 0.3f);
			// dde.setLod(0);
			// intersect(&dde, ray, aabb);
			// dde.setTransform(bunny);
			// dde.drawFrustum(mtxVp);
			// dde.push();
			// dde.pop();

			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded view ID
			bgfx::setState(BGFX_STATE_DEFAULT); // #TODO Remove?

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(viewIdFbo1, true);

			const bx::Vec3 start = { a_Start.x, a_Start.y, a_Start.z };
			const bx::Vec3 end = { a_End.x, a_End.y, a_End.z };
			debugDrawer.setColor(a_Color);
			debugDrawer.drawLine(start, end);
			debugDrawer.end();
		}

		void DrawSphere(const vec3f a_Position, const float a_Scale, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded view ID
			bgfx::setState(BGFX_STATE_DEFAULT); // #TODO Remove?

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(viewIdFbo1, true);
			debugDrawer.setColor(a_Color);
			debugDrawer.drawSphere(a_Position.x, a_Position.y, a_Position.z, a_Scale, Axis::X);
			debugDrawer.end();
		}

		void DrawCube(const vec3f a_Position, const float a_Size, const bool a_Wireframe, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded view ID
			bgfx::setState(BGFX_STATE_DEFAULT); // #TODO Remove?

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(viewIdFbo1, true);

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

		void DrawGrid(const vec3f a_Position, u16 a_Size, uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded view ID
			bgfx::setState(BGFX_STATE_DEFAULT);

			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(viewIdFbo1, true);

			constexpr bx::Vec3 normal = { .0f,  1.f, .0f };
			const bx::Vec3 pos = { a_Position.x, a_Position.y, a_Position.z };

			bx::Plane plane(bx::InitNone);
			bx::calcPlane(plane, normal, pos);

			debugDrawer.setColor(a_Color);
			debugDrawer.drawGrid(Axis::Y, pos, a_Size, 1.0f);
			debugDrawer.end();
		}

		void DrawOrb(uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

		void DrawCone(uint32_t a_Color)
		{
			const bool rendererInitialized = true; // #TODO Renderer should have a state bool or enum
			if (!rendererInitialized)
				return;

			// #TODO Implement
		}

	}

}
#endif // _QDEBUG
