#include <Render/ShaderProgramBuilder.h>

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
    auto vertex_shader = [this]() -> GLuint {
        if (_vertex_shader_text)
        {
            GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
            const auto vsh_text = _vertex_shader_text->data();
            glShaderSource(vertex_shader_id, 1, &vsh_text, nullptr);
            glCompileShader(vertex_shader_id);
            return vertex_shader_id;
        }
        throw std::runtime_error("ShaderProgramBuilder::build() : no vertex shader text added.");
    }();

    auto fragment_shader = [this]() -> GLuint {
        if (_fragment_shader_text)
        {
            GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
            const auto fsh_text = _fragment_shader_text->data();
            glShaderSource(fragment_shader_id, 1, &fsh_text, nullptr);
            glCompileShader(fragment_shader_id);
            return fragment_shader_id;
        }
        throw std::runtime_error("ShaderProgramBuilder::build() : no fragment shader text added.");
    }();

    auto program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    return program;
}