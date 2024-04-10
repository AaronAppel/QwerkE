#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QF_AssetsTypes.h"

namespace QwerkE {

	namespace Assets {

		void Initialize();
		void Shutdown();

		const bgfx::VertexBufferHandle& GetVbh();
		const bgfx::IndexBufferHandle& GetIbh();
		const bgfx::ProgramHandle& GetProgram();

		const Texture& GetTexture(const char* assetName);

#ifndef _QRETAIL
		// For editor only, really. Maybe find another use for exposing assets collections
		// map<TypeId, map<Id, Asset>> ViewAssets();
#endif

	}

}
