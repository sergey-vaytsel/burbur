#include <Render/Render.h>

#include <numbers>

#include <glad/glad.h>

Render::Render()
{
    gladLoadGL();

    // Render state
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); /* Uses default lighting parameters */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
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
