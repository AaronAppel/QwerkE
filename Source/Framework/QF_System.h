#pragma once

#include <string>

namespace QwerkE {

	namespace System {

		// #TODO Implement system or OS operations
		// void Sleep(double/float milliseconds/seconds);
		// void Yield();

		void Command(const std::string& a_Command);

		void StartProcess(const string& a_PathToProcess);

	}

}
