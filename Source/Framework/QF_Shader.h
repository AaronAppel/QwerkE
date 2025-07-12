#pragma once

#include <string>

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QC_GUID.h"

#include "QF_Buffer.h"
#include "QF_Files.h"
#include "QF_Paths.h"

namespace QwerkE {

	class Shader
	{
	public:
		// #TODO Can hide constructor and friend with Assets class
		Shader(const char* a_VertexShaderFilePath, const char* a_FragmentShaderFilePath, GUID a_Guid = GUID())
		{
			ASSERT(a_VertexShaderFilePath && a_FragmentShaderFilePath, "Invalid file name(s)!");
			m_GUID = a_Guid;
			m_VertexShaderFilePath = a_VertexShaderFilePath;
			m_FragmentShaderFilePath = a_FragmentShaderFilePath;
			Reload();
		}

		Shader::~Shader()
		{
			if (bgfx::isValid(m_ProgramHandle))
			{
				bgfx::destroy(m_ProgramHandle);
				m_ProgramHandle = BGFX_INVALID_HANDLE;
			}
		}

		// #TODO Consider hiding (private) for release builds?
		void Reload()
		{
			ASSERT(!m_VertexShaderFilePath.empty() && !m_FragmentShaderFilePath.empty(), "Invalid file name(s)!");

			if (bgfx::isValid(m_ProgramHandle))
			{
				bgfx::destroy(m_ProgramHandle);
			}

			// #TODO Better error handling

			bgfx::ShaderHandle m_VertexShader = BGFX_INVALID_HANDLE;
			bgfx::ShaderHandle m_FragmentShader = BGFX_INVALID_HANDLE;

			m_VertexFileBuffer.Release();
			m_VertexFileBuffer = Files::LoadFile(m_VertexShaderFilePath.c_str());
			if (m_VertexFileBuffer)
			{
				const bgfx::Memory* vertMemory = bgfx::makeRef(m_VertexFileBuffer.As<const void*>(), m_VertexFileBuffer.SizeInBytes() - 1);
				if (vertMemory)
				{
					m_VertexShader = bgfx::createShader(vertMemory);
				}
			}

			m_FragmentFileBuffer.Release();
			m_FragmentFileBuffer = Files::LoadFile(m_FragmentShaderFilePath.c_str());
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
				m_ProgramHandle = bgfx::createProgram(m_VertexShader, m_FragmentShader, false);
			}

			if (!bgfx::isValid(m_ProgramHandle))
			{
				ValidateVaryings(m_VertexShaderFilePath.c_str(), m_FragmentShaderFilePath.c_str());
			}

			ASSERT(m_InvalidBgfxId != m_ProgramHandle.idx && bgfx::isValid(m_ProgramHandle), "Invalid vertex and/or fragment shader!");

			if (bgfx::isValid(m_VertexShader))
			{
				bgfx::destroy(m_VertexShader);
			}
			if (bgfx::isValid(m_FragmentShader))
			{
				bgfx::destroy(m_FragmentShader);
			}
		}

		const bgfx::ProgramHandle& ProgramHandle() { return m_ProgramHandle; }
		const GUID& Guid() { return m_GUID; }

	private:
		void Shader::ValidateVaryings(const char* a_VertexFilePath, const char* a_FragmentFilePath);

		static constexpr unsigned short m_InvalidBgfxId = U16_MAX;
		bgfx::ProgramHandle m_ProgramHandle = BGFX_INVALID_HANDLE;

		std::string m_VertexShaderFilePath = "";
		std::string m_FragmentShaderFilePath = "";

		Buffer m_VertexFileBuffer; // #TODO bgfx defers instantiation so buffers can't be stack scoped to Load(). Look at avoiding holding extra RAM
		Buffer m_FragmentFileBuffer;

		GUID m_GUID = GUID::Invalid;
	};


}
