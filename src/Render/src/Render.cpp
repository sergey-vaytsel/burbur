#include <Render/Render.h>

#include <numbers>
#include <iostream>

#include <glad/glad.h>
#include <fmt/format.h>

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
        (void)length;
        (void)userParam;

        if (type != GL_DEBUG_TYPE_ERROR)
        {
            return;
        }

        std::cerr << fmt::format("GL CALLBACK: type = {}, severity = {}, message = {}}\n",
                                 type,
                                 severity,
                                 message);
    }

}

Render::Render(GLADloadproc glad_load_procedure)
{
    gladLoadGLLoader(glad_load_procedure);

    // Render state
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); /* Uses default lighting parameters */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_message_callback, 0);
}

void Render::update_viewport_size_if_needed(std::uint16_t width, std::uint16_t height)
{
    if (width == _width && height == _height)
    {
        return;
    }

    const double aspect_ratio = static_cast<double>(width) / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble fH = tan(fov / 360 * std::numbers::pi_v<float>) * z_near;
    GLdouble fW = fH * aspect_ratio;
    glFrustum(-fW, fW, -fH, fH, z_near, z_far);
    glViewport(0, 0, width, height);
}
