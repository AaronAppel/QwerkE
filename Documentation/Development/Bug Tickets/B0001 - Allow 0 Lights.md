QF_Scene.cpp
Removing all lights from a scene can cause exceptions and other undefined behaviour.
#TODO Refactor to support 0 lights in a scene.