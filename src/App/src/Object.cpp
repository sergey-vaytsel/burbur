#include "Object.h"
#include <stdexcept>

#include <glad/glad.h>

void ShaderProgram::add_fragment_shader(std::string &&text)
{
    _fragment_shader_text = std::move(text);
}

void ShaderProgram::add_vertex_shader(std::string &&text)
{
    _vertex_shader_text = std::move(text);
}

GLuint ShaderProgram::program()
{
    if (!_program)
    {
        compile();
    }
    return *_program;
}

void ShaderProgram::compile()
{
    auto vertex_shader = [this]() -> GLuint {
        if (_vertex_shader_text)
        {
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            const auto vsh_text = _vertex_shader_text->c_str();
            glShaderSource(vertex_shader, 1, &vsh_text, NULL);
            glCompileShader(vertex_shader);
            return vertex_shader;
        }
        throw std::runtime_error("ShaderProgram::compile() : no vertex shader text added.");
    }();

    auto fragment_shader = [this]() -> GLuint {
        if (_fragment_shader_text)
        {
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const auto fsh_text = _fragment_shader_text->c_str();
            glShaderSource(fragment_shader, 1, &fsh_text, NULL);
            glCompileShader(fragment_shader);
            return fragment_shader;
        }
        throw std::runtime_error("ShaderProgram::compile() : no fragment shader text added.");
    }();

    _program = glCreateProgram();
    glAttachShader(*_program, vertex_shader);
    glAttachShader(*_program, fragment_shader);
    glLinkProgram(*_program);
}