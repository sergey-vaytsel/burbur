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
#include "Object.h"

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
            "attribute vec3 vCol;\n"
            "attribute vec2 vPos;\n"
            "varying vec3 color;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
            "    color = vCol;\n"
            "}\n";

        auto fragment_shader_text =
            "#version 110\n"
            "varying vec3 color;\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = vec4(color, 1.0);\n"
            "}\n";

        return ShaderProgramBuilder()
            .add_fragment_shader(std::string(fragment_shader_text))
            .add_vertex_shader(std::string(vertex_shader_text))
            .build();
    }();
    glUseProgram(program);

    Assimp::Importer importer;
    std::string filename = "D:\\projects\\project\\test\\models\\3d\\box.uc";

    const aiScene *pScene = importer.ReadFile(
        filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
    {
        return -1;
    }

    {
        const auto vertices = pScene->mMeshes[0]->mVertices;
        const auto vertex_size = sizeof(vertices[0]);
        const auto vertices_count = pScene->mMeshes[0]->mNumVertices;
        GLuint vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertices_count * vertex_size, vertices, GL_STATIC_DRAW);

        GLint vpos_location;
        vpos_location = glGetAttribLocation(program, "vPos");
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                              vertex_size, (void *)0);
    }

    while (!window.shouldClose())
    {
        auto [width, height] = window.size();
        render.update_viewport_size(width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update material uniforms
        glm::mat4x4 m = glm::rotate(
            glm::mat4(1.0f),
            static_cast<GLfloat>(glfwGetTime()),
            glm::vec3(0.0f, 0.0f, 1.0f));
        auto ratio = static_cast<float>(width) / height;
        glm::mat4 v = glm::mat4(1.0f);
        glm::mat4x4 p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glm::mat4x4 mvp = p * v * m;

        GLint mvp_location = glGetUniformLocation(program, "MVP");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
