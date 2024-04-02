#pragma once

#include <functional>

namespace QwerkE {

	class Scriptable;

	class ComponentScript
	{
	public:
		Scriptable* Instance = nullptr;

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
