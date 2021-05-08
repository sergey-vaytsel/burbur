#include <string>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window {
public:
    ~Window();

    int init(unsigned width, unsigned height, const std::string & name);
    bool shouldClose() const;

    void clearAndSwap(GLbitfield clear_mask);
    void pollEvents();
private:
    GLFWwindow * glfw_window_ptr;
};