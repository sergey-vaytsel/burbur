#pragma once
#include <string>
#include <tuple>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window
{
public:
    ~Window();

    int init(unsigned width, unsigned height, const std::string &name);
    [[nodiscard]] bool shouldClose() const;

    void swapBuffers();
    void pollEvents();

    [[nodiscard]] std::tuple<std::uint16_t, std::uint16_t> size();

private:
    GLFWwindow *_glfw_window_ptr;
};
