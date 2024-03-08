#pragma once

using FramebufferResizedCallback = std::function<void(int width, int height)>;
using WindowResizedCallback = std::function<void(int width, int height)>;
using KeyCallback = std::function<void(int key, int scancode, int action, int mode)>;