All projects should utilize pre-compiled headers. Q\*\_PCH.h and .cpp files should exist in all projects. Here, externally referenced code should be added *if* it doesn't change often enough to slow down compilation times.

Forced includes are used to manage \#including PCH files. The project settings should include the appropriate Q\*\_ForcedIncludes.h file, as well as the PCH file separately so the PCH include order can be controlled without needing to change the forced include file :
![[{CFA9FE16-39A2-4331-A8C3-40F7805AB956}.png]]

The forced include file can be used to more easily include things in all files for a given project. The main usage is for defining custom types like Vec3, etc, and for defining aliases for existing types like uMap, sPtr, etc.