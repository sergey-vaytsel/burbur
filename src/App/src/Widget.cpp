#include "Widget.h"
#include <iostream>

#include <fmt/format.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

} // namespace

class Widget::WidgetImpl
{
public:
    ~WidgetImpl();

    int init(unsigned width, unsigned height, const std::string &name);
    [[nodiscard]] bool shouldClose() const;

    void swapBuffers();
    void pollEvents();

    [[nodiscard]] std::tuple<std::uint16_t, std::uint16_t> size();

private:
   GLFWwindow *_glfw_window_ptr;
};

Widget::WidgetImpl::~WidgetImpl()
{
    glfwDestroyWindow(_glfw_window_ptr);
    glfwTerminate();
}

int Widget::WidgetImpl::init(unsigned width, unsigned height, const std::string &name)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    _glfw_window_ptr = glfwCreateWindow(static_cast<int>(width),
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

    gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(this->_glfw_window_ptr, true);

    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    return 0;
}

bool Widget::WidgetImpl::shouldClose() const
{
    return glfwWindowShouldClose(_glfw_window_ptr);
}

void Widget::WidgetImpl::swapBuffers()
{
    glfwSwapBuffers(_glfw_window_ptr);
}

void Widget::WidgetImpl::pollEvents()
{
    glfwPollEvents();
}

std::tuple<std::uint16_t, std::uint16_t> Widget::WidgetImpl::size()
{
    int width, height;
    glfwGetFramebufferSize(_glfw_window_ptr, &width, &height);
    return {static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height)};
}


Widget::Widget()
    : _p_impl{std::make_unique<Widget::WidgetImpl>()}
{
}

Widget::~Widget() = default;

int Widget::init(unsigned width, unsigned height, const std::string &name)
{
    return this->_p_impl->init(width, height, name);
}

bool Widget::shouldClose() const
{
    return this->_p_impl->shouldClose();
}

void Widget::swapBuffers()
{
    this->_p_impl->swapBuffers();
}

void Widget::pollEvents()
{
    this->_p_impl->pollEvents();
}

std::tuple<std::uint16_t, std::uint16_t> Widget::size()
{
    return this->_p_impl->size();
}
