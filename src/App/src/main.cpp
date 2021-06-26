#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fmt/format.h>

#include <glad/glad.h>

#include "Window.h"
#include "Render.h"
#include "ShaderProgramBuilder.h"

int main(int, void **)
{
    Window window{};

    if (auto res = window.init(640, 480, "Project"))
    {
        std::cout << fmt::format("Init window error code: {}", res);
        return -1;
    }

    Render render;

    GLuint program = []
    {
        auto vertex_shader_text =
            "#version 110\n"
            "uniform mat4 MVP;\n"
            "attribute vec3 vPos;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = MVP * vec4(vPos, 1.0);\n"
            "}\n";

        auto fragment_shader_text =
            "#version 110\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = vec4(0.0, 1.0, 1.0, 0.5);\n"
            "}\n";

        return ShaderProgramBuilder()
            .add_fragment_shader(std::string(fragment_shader_text))
            .add_vertex_shader(std::string(vertex_shader_text))
            .build();
    }();
    glUseProgram(program);

    Assimp::Importer model_importer;
    std::string model_filename = "D:\\projects\\project\\test\\models\\3d\\box.uc";
    const aiScene *model_scene_ptr = model_importer.ReadFile(
        model_filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (model_scene_ptr == nullptr)
    {
        std::cout << fmt::format(
            "Import model '{}' error: {}",
            model_filename,
            model_importer.GetErrorString());
        return -1;
    }

    {
        const auto vertices = model_scene_ptr->mMeshes[0]->mVertices;
        const auto vertex_size = sizeof(vertices[0]);
        const auto vertices_count = model_scene_ptr->mMeshes[0]->mNumVertices;
        const auto vertices_size = vertices_count * vertex_size;

        std::cout << vertices_count << std::endl;
        for (auto i = 0u; i < vertices_count; i++)
        {
            const auto &vertex = vertices[i];
            std::cout << fmt::format("i={} v=({}, {}, {})", i, vertex.x, vertex.y, vertex.z) << std::endl;
        }

        GLuint vertex_buffer_id;
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

        const auto vertex_component_size = sizeof(vertices[0].x);
        const auto vertex_components_count = vertex_size / vertex_component_size;

        GLint vpos_location = glGetAttribLocation(program, "vPos");
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(
            vpos_location,
            vertex_components_count,
            GL_FLOAT,
            GL_FALSE,
            vertex_size,
            (void *)0);
    }

    while (!window.shouldClose())
    {
        auto [width, height] = window.size();
        render.update_viewport_size(width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // update material uniforms
        glm::mat4x4 m = glm::rotate(
            glm::mat4(1.0f),
            static_cast<GLfloat>(glfwGetTime()),
            glm::vec3(0.0f, 1.0f, 1.0f));
        auto ratio = static_cast<float>(width) / height;
        glm::mat4 v = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, -10.0f));
        glm::mat4x4 p = glm::perspective(static_cast<float>(M_PI) / 4.0f, ratio, 1.f, 1000.f);
        glm::mat4x4 mvp = p * v * m;

        GLint mvp_location = glGetUniformLocation(program, "MVP");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
