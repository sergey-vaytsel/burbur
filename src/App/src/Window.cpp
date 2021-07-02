#include "Window.h"
#include <iostream>

#include <fmt/format.h>

inline namespace
{

    void error_callback(const int error, const char *const description)
    {
        std::cerr << fmt::format("Error (Code={}): {}", error, description) << std::endl;
    }

    void key_callback(GLFWwindow *const window, const int key, const int, const int action, const int)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

} // inline namespace

Window::~Window()
{
    glfwDestroyWindow(_glfw_window_ptr);
    glfwTerminate();
}

int Window::init(unsigned width, unsigned height, const std::string &name)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    _glfw_window_ptr = glfwCreateWindow(
        static_cast<int>(width),
        static_cast<int>(height),
        name.c_str(),
        nullptr,
        nullptr);

    if (!_glfw_window_ptr)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(_glfw_window_ptr);
    glfwSwapInterval(1);

    glfwSetKeyCallback(_glfw_window_ptr, key_callback);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    return 0;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(_glfw_window_ptr);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(_glfw_window_ptr);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

std::tuple<std::uint16_t, std::uint16_t> Window::size()
{
    int width, height;
    glfwGetFramebufferSize(_glfw_window_ptr, &width, &height);
    return {
        static_cast<std::uint16_t>(width),
        static_cast<std::uint16_t>(height)};
}
