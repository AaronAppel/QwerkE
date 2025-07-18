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

			m_VertexFileBuffer.Release();
			m_VertexFileBuffer = Files::LoadFile(m_VertexShaderFilePath.c_str());
			if (m_VertexFileBuffer)
			{
				const bgfx::Memory* vertMemory = bgfx::makeRef(m_VertexFileBuffer.As<const void*>(), m_VertexFileBuffer.SizeInBytes() - 1);
				if (vertMemory)
				{
					m_VertexShaderHandle = bgfx::createShader(vertMemory);
				}
			}

			m_FragmentFileBuffer.Release();
			m_FragmentFileBuffer = Files::LoadFile(m_FragmentShaderFilePath.c_str());
			if (m_FragmentFileBuffer)
			{
				const bgfx::Memory* fragMemory = bgfx::makeRef(m_FragmentFileBuffer.As<const void*>(), m_FragmentFileBuffer.SizeInBytes() - 1,
					// #TODO Review delayed deletion. Maybe a Memory:: namespace could hold short term memory like buffers
					[](void* _ptr, void* _userData) {
						Buffer* buffer = (Buffer*)_userData;
						buffer->Release();
					},
				&m_FragmentFileBuffer);

				if (fragMemory)
				{
					m_FragmentShaderHandle = bgfx::createShader(fragMemory);
				}
			}

			if (bgfx::isValid(m_VertexShaderHandle) && bgfx::isValid(m_FragmentShaderHandle))
			{
				m_ProgramHandle = bgfx::createProgram(m_VertexShaderHandle, m_FragmentShaderHandle, false);
			}

			if (bgfx::isValid(m_ProgramHandle))
			{
				PrepUniformState();
			}
			else
			{
				ValidateVaryings(m_VertexShaderFilePath.c_str(), m_FragmentShaderFilePath.c_str());
			}

			if (m_InvalidBgfxId == m_ProgramHandle.idx || !bgfx::isValid(m_ProgramHandle))
			{
				LOG_ERROR("Vertex: {0}\nFragment: {1}", m_VertexShaderFilePath, m_FragmentShaderFilePath);
			}
			ASSERT(m_InvalidBgfxId != m_ProgramHandle.idx && bgfx::isValid(m_ProgramHandle), "Invalid vertex and/or fragment shader!");

			if (bgfx::isValid(m_VertexShaderHandle))
			{
				bgfx::destroy(m_VertexShaderHandle);
			}
			if (bgfx::isValid(m_FragmentShaderHandle))
			{
				bgfx::destroy(m_FragmentShaderHandle);
			}
		}

		bool HasTextureUniform() { return m_HasTextureUniform; }
		bool HasUniform(const char* a_UniformName);

		const bgfx::ProgramHandle& ProgramHandle() { return m_ProgramHandle; }
		const GUID& Guid() { return m_GUID; }

	private:
		void PrepUniformState();
		void Shader::ValidateVaryings(const char* a_VertexFilePath, const char* a_FragmentFilePath);

		static constexpr unsigned short m_InvalidBgfxId = U16_MAX;
		bgfx::ProgramHandle m_ProgramHandle = BGFX_INVALID_HANDLE;

		std::string m_VertexShaderFilePath = "";
		std::string m_FragmentShaderFilePath = "";

		bgfx::ShaderHandle m_VertexShaderHandle = BGFX_INVALID_HANDLE;
		bgfx::ShaderHandle m_FragmentShaderHandle = BGFX_INVALID_HANDLE;

		Buffer m_VertexFileBuffer; // #TODO bgfx defers instantiation so buffers can't be stack scoped to Load(). Look at avoiding holding extra RAM
		Buffer m_FragmentFileBuffer; // #TODO Pass a ReleaseFn to bgfx

		GUID m_GUID = GUID::Invalid;

		// #TODO Dynamic uniform handling
		bool m_HasTextureUniform = false;
	};


}
