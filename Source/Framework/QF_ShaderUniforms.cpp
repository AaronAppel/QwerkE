#include "QF_Shader.h"

namespace QwerkE {

	void Shader::PrepUniformState()
	{
		ASSERT(bgfx::isValid(m_ProgramHandle), "Invalid program handle!");

		{   // Vertex
			bgfx::UniformHandle handles[10];
			u16 numUniforms = bgfx::getShaderUniforms(m_VertexShaderHandle, handles, 10);

			for (size_t i = 0; i < numUniforms; i++)
			{
				bgfx::UniformInfo info;
				bgfx::getUniformInfo(handles[i], info);

				switch (info.type)
				{
				case bgfx::UniformType::Enum::Sampler:		//!< Sampler.
					break;
					// case case bgfx::UniformType::Enum::End:	//!< Reserved, do not use.
						// break;
				case bgfx::UniformType::Enum::Vec4:			//!< 4 floats vector.
					break;
				case bgfx::UniformType::Enum::Mat3:			//!< 3x3 matrix.
					break;
				case bgfx::UniformType::Enum::Mat4:			//!< 4x4 matrix.
					break;
				default:
					break;
				}
			}
		}

		{   // Fragment
			bgfx::UniformHandle handles[10];
			u16 numUniforms = bgfx::getShaderUniforms(m_FragmentShaderHandle, handles, 10);

			for (size_t i = 0; i < numUniforms; i++)
			{
				bgfx::UniformInfo info;
				bgfx::getUniformInfo(handles[i], info);

				switch (info.type)
				{
				case bgfx::UniformType::Enum::Sampler:		//!< Sampler.
					if (strcmp(info.name, "u_TextureColor") == 0)
					{
						m_HasTextureUniform = true;
					}
					else if (strcmp(info.name, "u_UVScaleOffset") == 0)
					{
						// #TODO Get or set in mesh
					}
					break;
					// case case bgfx::UniformType::Enum::End:	//!< Reserved, do not use.
						// break;
				case bgfx::UniformType::Enum::Vec4:			//!< 4 floats vector.
					break;
				case bgfx::UniformType::Enum::Mat3:			//!< 3x3 matrix.
					break;
				case bgfx::UniformType::Enum::Mat4:			//!< 4x4 matrix.
					break;
				default:
					break;
				}
			}
		}
	}

	bool Shader::HasUniform(const char* a_UniformName)
	{
		if (nullptr == a_UniformName)
		{
			LOG_ERROR("Null uniform name!");
			return false;
		}

		if (strlen(a_UniformName) < 3)
		{
			LOG_ERROR("Invalid uniform name: {0}", a_UniformName);
			return false;
		}

		if (!bgfx::isValid(m_ProgramHandle) || !bgfx::isValid(m_ProgramHandle) || !bgfx::isValid(m_ProgramHandle))
		{
			LOG_ERROR("Invalid shader or program handle(s)!");
			return false;
		}

		if (a_UniformName[0] != 'u' || a_UniformName[1] != '_')
		{
			LOG_ERROR("Invalid uniform prefix: {0}", a_UniformName);
			return false;
		}

		bgfx::UniformHandle handles[10];
		u16 numUniforms = bgfx::getShaderUniforms(m_VertexShaderHandle, handles, 10);

		for (size_t i = 0; i < numUniforms; i++)
		{
			bgfx::UniformInfo info;
			bgfx::getUniformInfo(handles[i], info);

			if (strcmp(info.name, a_UniformName) == 0)
			{
				return true;
			}
		}

		numUniforms = bgfx::getShaderUniforms(m_FragmentShaderHandle, handles, 10);

		for (size_t i = 0; i < numUniforms; i++)
		{
			bgfx::UniformInfo info;
			bgfx::getUniformInfo(handles[i], info);

			if (strcmp(info.name, a_UniformName) == 0)
			{
				return true;
			}
		}
		return false;
	}

}
