Also see [[RCCpp Runtime Compilation]]

The MVP RCCpp example is limited to recompiling the RCCppMainLoop.cpp::MainLoop() function only at the moment. This could change in the future, to support recompiling a section the Framework or Editor, but the cost to benefit ratio is yet to be seen.

For now, MainLoop() is setup to play with. Note that the imgui context seems to be destroyed or non-accessible after a recompile, so that is an issue that would need addressing.