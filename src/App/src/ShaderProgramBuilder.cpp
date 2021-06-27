#include "ShaderProgramBuilder.h"

#include <stdexcept>

#include <glad/glad.h>

ShaderProgramBuilder &ShaderProgramBuilder::add_fragment_shader(std::string &&text)
{
    _fragment_shader_text = std::move(text);
    return *this;
}

ShaderProgramBuilder &ShaderProgramBuilder::add_vertex_shader(std::string &&text)
{
    _vertex_shader_text = std::move(text);
    return *this;
}

GLuint ShaderProgramBuilder::build()
{
    auto vertex_shader = [this]() -> GLuint
    {
        if (_vertex_shader_text)
        {
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            const auto vsh_text = _vertex_shader_text->data();
            glShaderSource(vertex_shader, 1, &vsh_text, NULL);
            glCompileShader(vertex_shader);
            return vertex_shader;
        }
        throw std::runtime_error("ShaderProgramBuilder::build() : no vertex shader text added.");
    }();

    auto fragment_shader = [this]() -> GLuint
    {
        if (_fragment_shader_text)
        {
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const auto fsh_text = _fragment_shader_text->data();
            glShaderSource(fragment_shader, 1, &fsh_text, NULL);
            glCompileShader(fragment_shader);
            return fragment_shader;
        }
        throw std::runtime_error("ShaderProgramBuilder::build() : no fragment shader text added.");
    }();

    auto program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    return program;
}