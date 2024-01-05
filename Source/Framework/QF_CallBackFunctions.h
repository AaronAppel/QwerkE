#pragma once

struct GLFWwindow;

namespace QwerkE {

    void SetupCallbacks(GLFWwindow* window);

    void window_resized_callback(GLFWwindow* window, int width, int height);
    void window_closed_callback(GLFWwindow* window);

    void error_callback(int error, const char* description);

    void file_drop_callback(GLFWwindow* window, int count, const char** paths);

}
