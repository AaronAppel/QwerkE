#pragma once

namespace QwerkE {

	namespace Editor {

		void Run(unsigned int argc, char** argv);
		void Stop();

		bool StillRunning();

		void OnReset();

		void RunBgfxExample();
	}

}