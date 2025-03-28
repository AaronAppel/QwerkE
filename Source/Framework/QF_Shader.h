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

		// private: // #TODO Hide as members?
		bgfx::ProgramHandle m_Program;
		GUID m_GUID;
	};


}
