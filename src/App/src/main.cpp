#include <filesystem>
#include <fstream>
#include <iostream>
#include <numbers>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fmt/format.h>

#include <glad/glad.h>

#include <Model/Model.h>
#include <Render/Camera.h>
#include <Render/Render.h>
#include <Render/Shader.h>

#include "Window.h"

namespace fs = std::filesystem;

//! /todo copy bin, test to binaries
// {PROJECT_DIR}/build/src/App/App.exe
const auto g_project_path = fs::current_path() / "../../..";

int main(int, char **)
{
    //! /todo imgui
    Window window{};

    if (auto res = window.init(640, 480, "Project"))
    {
        std::cout << fmt::format("Init window error code: {}", res);
        return -1;
    }

    render::Renderer render{GLADloadproc(glfwGetProcAddress)};
    render::Camera camera;
    auto shader = [] {
        const auto shader_path = g_project_path / "bin/shaders";
        const auto vertex_shader_file_path = shader_path / "default.vsh";
        const auto fragment_shader_file_path = shader_path / "default.fsh";
        return render::DefaultShader(vertex_shader_file_path, fragment_shader_file_path);
    }();
    shader.use();

    model::Model model{g_project_path / "test/models/AC/Wuson.ac"};

    const auto &mesh = model.meshes()[0];
    const auto vertices_count = mesh.vertices.size();
    {
        //! /todo model scene node

        const auto vertices = mesh.vertices;
        constexpr auto vertex_size = sizeof(vertices[0]);
        const auto vertices_size = vertices_count * vertex_size;

        const auto normals = mesh.normals;
        constexpr auto normal_size = sizeof(normals[0]);
        const auto normals_count = normals.size();
        const auto normals_size = normals_count * normal_size;

        GLuint vertex_buffer_ids[2];
        glGenBuffers(2, vertex_buffer_ids);

        GLuint vertex_array_id;
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);

        const auto attribute_position_location = shader.location(render::Attribute::POSITION_VEC3);
        const auto attribute_normal_location = shader.location(render::Attribute::NORMAL_VEC3);
        glEnableVertexAttribArray(attribute_position_location);
        glEnableVertexAttribArray(attribute_normal_location);

        constexpr auto vertex_components_count = vertex_size / sizeof(vertices[0].x);
        constexpr auto normal_components_count = normal_size / sizeof(normals[0].x);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ids[0]);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices_size),
                     vertices.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer(attribute_position_location,
                              static_cast<GLint>(vertex_components_count),
                              GL_FLOAT,
                              GL_FALSE,
                              vertex_size,
                              static_cast<void *>(0));

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ids[1]);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(normals_size),
                     normals.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer(attribute_normal_location,
                              static_cast<GLint>(normal_components_count),
                              GL_FLOAT,
                              GL_FALSE,
                              normal_size,
                              static_cast<void *>(0));

        //! /todo use index buffer
    }

    const auto light_position = glm::vec3{5.0f, 5.0f, 1.0f};
    const auto light_color = glm::vec3{1.0f, 1.0f, 1.0f};
    const auto object_color = glm::vec3{1.0f, 0.5f, 0.31f};

    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.look_at(glm::vec3(0.0f, 0.5f, 0.0f));

    shader.set_uniform(render::Uniform::LIGHT_POS_VEC3, light_position);
    shader.set_uniform(render::Uniform::LIGHT_COLOR_VEC3, light_color);
    shader.set_uniform(render::Uniform::OBJECT_COLOR_VEC3, object_color);

    while (!window.shouldClose())
    {
        auto [width, height] = window.size();
        auto ratio = static_cast<float>(width) / height;
        render.update_viewport_size_if_needed(width, height);
        camera.set_aspect_ratio_if_needed(ratio);
        render.clear();

        auto model_matrix = [] {
            auto res = glm::mat4(1.0f);
            res = glm::rotate(res, std::numbers::pi_v<float> / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            res =
                glm::rotate(res, static_cast<GLfloat>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
            return res;
        }();
        shader.set_uniform(render::Uniform::MODEL_MATRIX, model_matrix);

        //! /todo camera controller
        shader.set_uniform(render::Uniform::VIEW_MATRIX, camera.view());
        shader.set_uniform(render::Uniform::PROJECTION_MATRIX, camera.projection());
        shader.set_uniform(render::Uniform::CAMERA_POS_VEC3, camera.position());

        //! /todo render.draw(scene);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_count));

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
