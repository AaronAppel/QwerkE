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

		void DrawLine(const vec3f a_Start, const vec3f a_End);
		void DrawSphere(const vec3f a_Position, const float a_Scale = 1.f);
		void DrawCube();
		void DrawCylinder();
		void DrawGrid(const vec3f a_Position, u16 a_Size);
	}

}

#else
#define BREAK			(void)0
#define ASSERT(x, msg)	(void)0
#endif
