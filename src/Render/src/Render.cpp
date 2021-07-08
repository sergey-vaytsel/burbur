#include <Render/Render.h>

#include <iostream>
#include <numbers>

#include <fmt/format.h>
#include <glad/glad.h>

namespace render
{

inline namespace
{
void GLAPIENTRY gl_message_callback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar *message,
                                    const void *userParam)
{
    (void)source;
    (void)id;
    (void)severity;
    (void)userParam;

    if (type != GL_DEBUG_TYPE_ERROR)
    {
        return;
    }

    std::cerr << fmt::format("GL CALLBACK: {}\n",
                             std::string{message, static_cast<size_t>(length)});
}

} // namespace

Renderer::Renderer(GLADloadproc glad_load_procedure)
{
    gladLoadGLLoader(glad_load_procedure);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_message_callback, 0);
}

void Renderer::update_viewport_size_if_needed(std::uint16_t width, std::uint16_t height)
{
    if (width == _width && height == _height)
    {
        return;
    }
    glViewport(0, 0, width, height);
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

} // namespace render
