#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/format.h>

#include "Window.h"
#include "Render.h"
#include "Object.h"

int main(void)
{
    Window window{};
    if (auto res = window.init(640, 480, "Project"))
    {
        std::cout << fmt::format("Init window error code: {}", res);
        return -1;
    }

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

        // compile material shader program
        GLuint vertex_shader, fragment_shader;

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

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
        window.clear(GL_COLOR_BUFFER_BIT);

        auto [width, height] = window.wh();

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
