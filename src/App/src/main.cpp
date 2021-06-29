#include <numbers>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fmt/format.h>

#include <glad/glad.h>

#include <Render/Camera.h>
#include <Render/Render.h>
#include <Render/ShaderProgramBuilder.h>

#include "Window.h"

namespace fs = std::filesystem;

// {PROJECT_DIR}/build/src/App/App.exe
const auto g_project_path = fs::current_path() / "../../..";

int main(int, void **)
{
    Window window{};

    if (auto res = window.init(640, 480, "Project"))
    {
        std::cout << fmt::format("Init window error code: {}", res);
        return -1;
    }

    Render render;
    Camera camera;

    GLuint program = []
    {
        const auto shader_path = g_project_path / "bin/shaders";
        auto vertex_shader_file_path = shader_path / "default.vsh";
        auto fragment_shader_file_path = shader_path / "default.fsh";

        const auto file_content = [](fs::path &&path) -> std::string
        {
            std::stringstream stream;
            std::ifstream(path) >> stream.rdbuf();
            return stream.str();
        };

        return ShaderProgramBuilder()
            .add_vertex_shader(file_content(std::move(vertex_shader_file_path)))
            .add_fragment_shader(file_content(std::move(fragment_shader_file_path)))
            .build();
    }();
    glUseProgram(program);

    Assimp::Importer model_importer;
    const auto model_filename = g_project_path / "test/models/AC/Wuson.ac";
    const aiScene *model_scene_ptr = model_importer.ReadFile(
        model_filename.string(),
        0);

    if (model_scene_ptr == nullptr)
    {
        std::cout << fmt::format(
            "Import model '{}' error: {}",
            model_filename.string(),
            model_importer.GetErrorString());
        return -1;
    }

    const auto vertices = model_scene_ptr->mMeshes[0]->mVertices;
    const auto vertex_size = sizeof(vertices[0]);
    const auto vertices_count = model_scene_ptr->mMeshes[0]->mNumVertices;
    const auto vertices_size = vertices_count * vertex_size;

    {
        GLuint vertex_buffer_id;
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

        const auto vertex_component_size = sizeof(vertices[0].x);
        const auto vertex_components_count = vertex_size / vertex_component_size;

        GLint vpos_location = glGetAttribLocation(program, "a_vec3_position");
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(
            vpos_location,
            vertex_components_count,
            GL_FLOAT,
            GL_FALSE,
            vertex_size,
            (void *)0);
    }

    camera.set_position(glm::vec3(.0f, -1.f, -5.0f));

    while (!window.shouldClose())
    {
        auto [width, height] = window.size();
        auto ratio = static_cast<float>(width) / height;
        render.update_viewport_size_if_needed(width, height);
        camera.set_aspect_ratio_if_needed(ratio);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4x4 model_matrix = glm::rotate(
            glm::mat4(1.0f),
            static_cast<GLfloat>(glfwGetTime()),
            glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4x4 mvp = camera.view_projection() * model_matrix;
        GLint mvp_location = glGetUniformLocation(program, "u_mat4_mvp");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glDrawArrays(GL_LINES, 0, vertices_count);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
