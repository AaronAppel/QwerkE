#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

namespace QwerkE {

	enum ShaderComponentTypes
	{
		e_None = 0,
		e_Vertex,
		e_Fragment
	};

	class ShaderComponent // #TODO Review for deprecation
	{
	public:
		ShaderComponent::~ShaderComponent()
		{
			bgfx::destroy(m_Handle);
		}

		// private: // #TODO Hide as members?
		bgfx::ShaderHandle m_Handle = BGFX_INVALID_HANDLE;
		GUID m_GUID;
		ShaderComponentTypes m_ShaderComponentType = ShaderComponentTypes::e_None;
	};

}
