#pragma once

namespace QwerkE {

	class Scriptable;
	enum eScriptTypes : u8;

	namespace Scripting {

		Scriptable* InstantiateScript(eScriptTypes scriptType);

	}

}
