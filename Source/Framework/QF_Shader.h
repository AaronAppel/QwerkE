#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

namespace QwerkE {

	class Shader
	{
	public:
		Shader::~Shader()
		{
			bgfx::destroy(m_Program);
		}

		// #TODO Look at moving shader creation logic here or into a ShaderFactory type class/namespace

		// private: // #TODO Hide as members?
		bgfx::ProgramHandle m_Program = BGFX_INVALID_HANDLE;

		GUID m_GUID = GUID::Invalid;
	};


}
