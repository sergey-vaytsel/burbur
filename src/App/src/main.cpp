#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/format.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Render.h"
#include "Object.h"

void reshape(int width, int height)
{
    const double aspect_ratio = static_cast<double>(width) / height;
    const double fov = 45.0;
    const double z_near = 1.0;
    const double z_far = 1000.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble fH = tan(fov / 360 * M_PI) * z_near;
    GLdouble fW = fH * aspect_ratio;
    glFrustum(-fW, fW, -fH, fH, z_near, z_far);
    glViewport(0, 0, width, height);
}

int main(int, void **)
{
    Window window{};

    if (auto res = window.init(640, 480, "Project"))
    {
        std::cout << fmt::format("Init window error code: {}", res);
        return -1;
    }

    gladLoadGL();

    GLuint program;

    {
        // create Mesh
        static const struct
        {
            float x, y;
            float r, g, b;
        } vertices[3] =
            {
                {-0.6f, -0.4f, 1.f, 0.f, 0.f},
                {0.6f, -0.4f, 0.f, 1.f, 0.f},
                {0.f, 0.6f, 0.f, 0.f, 1.f}};

        GLuint vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // create material
        static const char *vertex_shader_text =
            "#version 110\n"
            "uniform mat4 MVP;\n"
            "attribute vec3 vCol;\n"
            "attribute vec2 vPos;\n"
            "varying vec3 color;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
            "    color = vCol;\n"
            "}\n";

        static const char *fragment_shader_text =
            "#version 110\n"
            "varying vec3 color;\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = vec4(color, 1.0);\n"
            "}\n";

        ShaderProgram shader_program;
        shader_program.add_fragment_shader(std::string(fragment_shader_text));
        shader_program.add_vertex_shader(std::string(vertex_shader_text));
        program = shader_program.program();

        // bind vertex attributes
        GLint vpos_location, vcol_location;
        vpos_location = glGetAttribLocation(program, "vPos");
        vcol_location = glGetAttribLocation(program, "vCol");

        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                              sizeof(vertices[0]), (void *)0);
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                              sizeof(vertices[0]), (void *)(sizeof(float) * 2));
    }

    while (!window.shouldClose())
    {
        auto [width, height] = window.wh();
        reshape(width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // use material shader program
        glUseProgram(program);

        // update material uniforms
        GLint mvp_location = glGetUniformLocation(program, "MVP");
        glm::mat4x4 m = glm::rotate(glm::mat4(1.0f), static_cast<GLfloat>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
        auto ratio = static_cast<float>(width) / height;
        glm::mat4x4 p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glm::mat4x4 mvp = p * m;
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        //draw mesh
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
