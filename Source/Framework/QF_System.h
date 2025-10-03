#pragma once

#include <string>

namespace QwerkE {

	namespace System {

		std::string UserName();
		
		// void Sleep(double/float milliseconds/seconds);
		// #TODO ::Sleep(milliseconds);

		void YieldTime(); // #TODO Rename "Yield" after fixing WinBase.h include name collision

		void Command(const std::string& a_Command);

		void StartProcess(const string& a_PathToProcess);

	}

}
