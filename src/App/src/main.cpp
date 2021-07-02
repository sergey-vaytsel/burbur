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

int main(int, char **)
{
    Window window{};

    if (auto res = window.init(640, 480, "Project"))
    {
        std::cout << fmt::format("Init window error code: {}", res);
        return -1;
    }

    Render render{GLADloadproc(glfwGetProcAddress)};
    Camera camera;

    const auto program = []
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
    const auto uniform_model_matrix_name = "u_mat4_model";
    const auto uniform_view_matrix_name = "u_mat4_view";
    const auto uniform_projection_matrix_name = "u_mat4_projection";

    const auto uniform_light_position_name = "u_vec3_light_position";
    const auto uniform_camera_position_name = "u_vec3_camera_position";
    const auto uniform_light_color_name = "u_vec3_light_color";
    const auto uniform_object_color_name = "u_vec3_object_color";

    const auto attribute_position_location = 0;
    const auto attribute_normal_location = 1;

    glUseProgram(program);

    const auto uniform_light_position_location = glGetUniformLocation(program, uniform_light_position_name);
    const auto uniform_light_color_location = glGetUniformLocation(program, uniform_light_color_name);
    const auto uniform_object_color_location = glGetUniformLocation(program, uniform_object_color_name);

    const auto uniform_model_matrix_location = glGetUniformLocation(program, uniform_model_matrix_name);
    const auto uniform_view_matrix_location = glGetUniformLocation(program, uniform_view_matrix_name);
    const auto uniform_projection_matrix_location = glGetUniformLocation(program, uniform_projection_matrix_name);
    const auto uniform_camera_position_location = glGetUniformLocation(program, uniform_camera_position_name);

    Assimp::Importer model_importer;
    const auto model_filename = g_project_path / "test/models/AC/Wuson.ac";
    const auto model_scene_ptr = model_importer.ReadFile(
        model_filename.string(),
        aiProcess_Triangulate | aiProcess_GenNormals);

    if (model_scene_ptr == nullptr)
    {
        std::cout << fmt::format(
            "Import model '{}' error: {}",
            model_filename.string(),
            model_importer.GetErrorString());
        return -1;
    }

    const auto vertices = model_scene_ptr->mMeshes[0]->mVertices;
    constexpr auto vertex_size = sizeof(vertices[0]);
    const auto vertices_count = model_scene_ptr->mMeshes[0]->mNumVertices;
    const auto vertices_size = vertices_count * vertex_size;

    const auto normals = model_scene_ptr->mMeshes[0]->mNormals;
    constexpr auto normal_size = sizeof(normals[0]);
    const auto normals_count = model_scene_ptr->mMeshes[0]->mNumVertices;
    const auto normals_size = normals_count * normal_size;

    {
        GLuint vertex_buffer_ids[2];
        glGenBuffers(2, vertex_buffer_ids);

        GLuint vertex_array_id;
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);

        glEnableVertexAttribArray(attribute_position_location);
        glEnableVertexAttribArray(attribute_normal_location);

        constexpr auto vertex_component_size = sizeof(vertices[0].x);
        constexpr auto vertex_components_count = vertex_size / vertex_component_size;
        constexpr auto normals_component_size = sizeof(normals[0].x);
        constexpr auto normals_components_count = normal_size / normals_component_size;

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ids[0]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_size), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(
            attribute_position_location,
            static_cast<GLint>(vertex_components_count),
            GL_FLOAT,
            GL_FALSE,
            vertex_size,
            static_cast<void *>(0));

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ids[1]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(normals_size), normals, GL_STATIC_DRAW);
        glVertexAttribPointer(
            attribute_normal_location,
            static_cast<GLint>(normals_components_count),
            GL_FLOAT,
            GL_FALSE,
            normal_size,
            static_cast<void *>(0));
    }

    const auto light_position = glm::vec3{5.0f, 5.0f, 1.0f};
    const auto light_color = glm::vec3{1.0f, 1.0f, 1.0f};
    const auto object_color = glm::vec3{1.0f, 0.5f, 0.31f};

    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.look_at(glm::vec3(0.0f, 0.5f, 0.0f));

    glUniform3f(uniform_light_position_location, light_position.x, light_position.y, light_position.z);
    glUniform3f(uniform_light_color_location, light_color.x, light_color.y, light_color.z);
    glUniform3f(uniform_object_color_location, object_color.x, object_color.y, object_color.z);

    while (!window.shouldClose())
    {
        auto [width, height] = window.size();
        auto ratio = static_cast<float>(width) / height;
        render.update_viewport_size_if_needed(width, height);
        camera.set_aspect_ratio_if_needed(ratio);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto model_matrix = glm::mat4(1.0f);
        model_matrix = glm::rotate(
            model_matrix,
            std::numbers::pi_v<float> / 2,
            glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(
            model_matrix,
            static_cast<GLfloat>(glfwGetTime()),
            glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniform_model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(uniform_view_matrix_location, 1, GL_FALSE, glm::value_ptr(camera.view()));
        glUniformMatrix4fv(uniform_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(camera.projection()));
        const auto camera_position = camera.position();
        glUniform3f(uniform_camera_position_location, camera_position.x, camera_position.y, camera_position.z);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_count));

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
