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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    _glfw_window_ptr = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (!_glfw_window_ptr)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(_glfw_window_ptr);
    glfwSwapInterval(1);
    gladLoadGL();

    glfwSetKeyCallback(_glfw_window_ptr, key_callback);

    return 0;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(_glfw_window_ptr);
}

void Window::clear(GLbitfield clear_mask)
{
    glfwGetFramebufferSize(_glfw_window_ptr, &_width, &_height);
    glViewport(0, 0, _width, _height);
    glClear(clear_mask);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(_glfw_window_ptr);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

int Window::width() { return _width; }
int Window::height() { return _height; }
std::tuple<int, int> Window::wh() { return {_width, _height}; };