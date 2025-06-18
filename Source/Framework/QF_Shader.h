#pragma once

#include <string>

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QC_GUID.h"

#include "QF_Files.h"
#include "QF_Paths.h"

namespace QwerkE {

	class Shader
	{
	public:
		Shader(const char* a_VertexShaderFileName, const char* a_FragmentShaderFileName, GUID a_Guid = GUID())
		{
			ASSERT(a_VertexShaderFileName && a_FragmentShaderFileName, "Invalid file name(s)!");
			m_GUID = a_Guid;
			m_VertexShaderFileName = a_VertexShaderFileName;
			m_FragmentShaderFileName = a_FragmentShaderFileName;
			Reload();
		}

		Shader::~Shader()
		{
			bgfx::destroy(m_Program);
		}

		// #TODO Consider hiding (private) for release builds?
		void Reload()
		{
			ASSERT(!m_VertexShaderFileName.empty() && !m_FragmentShaderFileName.empty(), "Invalid file name(s)!");

			if (bgfx::isValid(m_Program))
			{
				bgfx::destroy(m_Program);
			}

			// #TODO Better error handling

			bgfx::ShaderHandle m_VertexShader = BGFX_INVALID_HANDLE;
			bgfx::ShaderHandle m_FragmentShader = BGFX_INVALID_HANDLE;

			m_VertexFileBuffer.Release();
			m_VertexFileBuffer = Files::LoadFile(Paths::Shader(m_VertexShaderFileName.c_str()).c_str());
			if (m_VertexFileBuffer)
			{
				const bgfx::Memory* vertMemory = bgfx::makeRef(m_VertexFileBuffer.As<const void*>(), m_VertexFileBuffer.SizeInBytes() - 1);
				if (vertMemory)
				{
					m_VertexShader = bgfx::createShader(vertMemory);
				}
			}

			m_FragmentFileBuffer.Release();
			m_FragmentFileBuffer = Files::LoadFile(Paths::Shader(m_FragmentShaderFileName.c_str()).c_str());
			if (m_FragmentFileBuffer)
			{
				const bgfx::Memory* fragMemory = bgfx::makeRef(m_FragmentFileBuffer.As<const void*>(), m_FragmentFileBuffer.SizeInBytes() - 1);
				if (fragMemory)
				{
					m_FragmentShader = bgfx::createShader(fragMemory);
				}
			}

			if (bgfx::isValid(m_VertexShader) && bgfx::isValid(m_FragmentShader))
			{
				m_Program = bgfx::createProgram(m_VertexShader, m_FragmentShader, false);
			}

			if (bgfx::isValid(m_VertexShader))
			{
				bgfx::destroy(m_VertexShader);
			}
			if (bgfx::isValid(m_FragmentShader))
			{
				bgfx::destroy(m_FragmentShader);
			}
		}

		const bgfx::ProgramHandle& Program() { return m_Program; }
		const GUID& Guid() { return m_GUID; }

	private:
		bgfx::ProgramHandle m_Program = BGFX_INVALID_HANDLE;

		std::string m_VertexShaderFileName = "";
		std::string m_FragmentShaderFileName = "";

		Buffer m_VertexFileBuffer; // #TODO bgfx defers instantiation so buffers can't be stack scoped to Load(). Look at avoiding holding extra RAM
		Buffer m_FragmentFileBuffer;

		GUID m_GUID = GUID::Invalid;
	};


}
