#pragma once

#include "QF_AssetsTypes.h"

namespace QwerkE {

	namespace Assets {

		const Texture& GetTexture(const char* assetName);

#ifndef _QRETAIL
		// For editor only, really. Maybe find another use for exposing assets collections
		// map<TypeId, map<Id, Asset>> ViewAssets();
#endif

	}

}
