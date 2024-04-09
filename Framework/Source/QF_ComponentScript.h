#pragma once

#include <functional>

namespace QwerkE {

	enum eScriptTypes;
	class Scriptable;

	class ComponentScript
	{
	public:
		Scriptable* Instance = nullptr;
		eScriptTypes m_ScriptType = (eScriptTypes)0;

		Scriptable* (*InstantiateScript)();
		void (*DeleteScript)(ComponentScript*);

		template <typename T>
		void Bind()
		{
			InstantiateScript = []()
				{
					return static_cast<Scriptable*>(new T());
				};

			DeleteScript = [](ComponentScript* nsc)
				{
					delete (T*)nsc->Instance;
					nsc->Instance = nullptr;
				};
		}

	};

}
