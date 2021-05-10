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
    bool shouldClose() const;

    void clear(GLbitfield clear_mask);
    void swapBuffers();
    void pollEvents();

    int width();
    int height();
    std::tuple<int, int> wh();

private:
    GLFWwindow *_glfw_window_ptr;
    int _width;
    int _height;
};
