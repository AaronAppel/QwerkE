#pragma once


#ifdef _QGLM
#include "Libraries/glm/glm.hpp"
typedef glm::vec3		vec3f;
#endif

#ifdef _QDEBUG

#include "QF_Log.h"

#define BREAK __debugbreak();

#define ASSERT(x, msg)   \
if ((x)) { } else { LOG_CRITICAL("Assert! {0}, {1}, in {2}() in {3}({4})", #x, msg, __FUNCTION__, __FILE__, __LINE__); BREAK }

#define NULL_ARG_CHECK_RETURN(value, ret) if (!value) { LOG_ERROR("{0} Null argument passed!", __FUNCTION__); return ret; }
#define NULL_ARG_CHECK_RETURN2(value1, value2, ret) if (!value1 || !value2) { LOG_ERROR("{0} Null argument passed!", __FUNCTION__); return ret; }

namespace QwerkE {

	namespace Debug {

		const uint32_t g_White = 0xffffffff;
		const uint32_t g_Black = 0xff000000;

		const uint32_t g_Red = 0xff0000ff; // #NOTE ABGR
		const uint32_t g_Green = 0xff00ff00;
		const uint32_t g_Blue = 0xffff0000;

		const uint32_t g_Cyan = 0xfffafa0a;
		const uint32_t g_Magenta = 0xffff00ff;
		const uint32_t g_Yellow = 0xff80ffff;

		const uint32_t g_Purple = 0xff7b0c5b; // #TODO Find a good purple
		const uint32_t g_Orange = 0xff3357ff;
		const uint32_t g_Brown = 0xff072766;

		// #TODO Opacity helpers
		// Darker pink: 0xff7d3c98
		// Royal blue: 0xffFF5733, (darker):0xffa44211

		// #TODO Add a_Wireframe argument to more shape functions
		// #TODO Take larger arguments in by reference, but also keep convenience of r values

		void DrawArc(const uint32_t a_Color = g_White);
		void DrawAxis(const uint32_t a_Color = g_White);
		void DrawCapsule(const uint32_t a_Color = g_White);
		void DrawCircle(const uint32_t a_Color = g_White);
		void DrawCone(const uint32_t a_Color = g_White);
		void DrawCube(const vec3f a_Position, const float a_Size = 1.f, const bool a_Wireframe = false, const uint32_t a_Color = g_White);
		void DrawCylinder(const uint32_t a_Color = g_White);
		void DrawFrustum(const uint32_t a_Color = g_White);
		void DrawGrid(const vec3f a_Position, const u16 a_Size, const uint32_t a_Color = g_White);

		void DrawLine(const vec3f a_Start, const vec3f a_End, const uint32_t a_Color = g_White);

		void DrawOrb(const uint32_t a_Color = g_White);

		void DrawSphere(const vec3f a_Position, const float a_Scale = 1.f, const uint32_t a_Color = g_White);

	}

}

#else
#define BREAK			(void)0
#define ASSERT(x, msg)	(void)0
#endif
