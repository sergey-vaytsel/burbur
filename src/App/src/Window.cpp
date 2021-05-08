#include "Window.h"
#include <iostream>



inline namespace {

void error_callback(const int error, const char * const description)
{
    std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow * const window, const int key, const int scancode, const int action, const int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

} // inline namespace

Window::~Window() {
    glfwDestroyWindow(glfw_window_ptr);
    glfwTerminate();
}

int Window::init(unsigned width, unsigned height, const std::string & name) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfw_window_ptr = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!glfw_window_ptr)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(glfw_window_ptr);
    glfwSwapInterval(1);
    gladLoadGL();

    glfwSetKeyCallback(glfw_window_ptr, key_callback);

    return 0;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(glfw_window_ptr);
}

void Window::clearAndSwap(GLbitfield clear_mask) {
    glClear(clear_mask);

    glfwSwapBuffers(glfw_window_ptr);
}

void Window::pollEvents() {
    glfwPollEvents();
}
