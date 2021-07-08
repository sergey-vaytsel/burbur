#include <Render/Shader.h>
#include <Render/ShaderProgramBuilder.h>

#include <fstream>
#include <iostream>

namespace render
{

DefaultShader::DefaultShader(const fs::path &vertex_shader_file_path,
                             const fs::path &fragment_shader_file_path)
{
    const auto file_content = [](const fs::path &path) -> std::string {
        std::stringstream stream;
        std::ifstream(path) >> stream.rdbuf();
        return stream.str();
    };

    _program = ShaderProgramBuilder()
                   .add_vertex_shader(file_content(vertex_shader_file_path))
                   .add_fragment_shader(file_content(fragment_shader_file_path))
                   .build();
}

DefaultShader::~DefaultShader()
{
    glDeleteProgram(_program);
}

IShader::AttributeLocations DefaultShader::attribute_locations()
{
    //! /todo reflection + materials config
    if (cached_attribute_locations == std::nullopt)
    {
        cached_attribute_locations = {{
            {Attribute::POSITION_VEC3, 0},
            {Attribute::NORMAL_VEC3, 1},
        }};
    }
    return cached_attribute_locations.value();
}

IShader::UniformLocations DefaultShader::uniform_locations()
{
    if (cached_uniform_locations == std::nullopt)
    {
        GLint current_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        if (_program != static_cast<GLuint>(current_program))
        {
            std::cerr << "ERROR: can not get uniform locations of unused program";
            return {};
        }
        //! /todo reflection + materials config
        constexpr auto uniform_model_matrix_name = "u_mat4_model";
        constexpr auto uniform_view_matrix_name = "u_mat4_view";
        constexpr auto uniform_projection_matrix_name = "u_mat4_projection";
        constexpr auto uniform_light_position_name = "u_vec3_light_position";
        constexpr auto uniform_light_color_name = "u_vec3_light_color";
        constexpr auto uniform_camera_position_name = "u_vec3_camera_position";
        constexpr auto uniform_object_color_name = "u_vec3_object_color";
        cached_uniform_locations = {{
            {Uniform::MODEL_MATRIX, glGetUniformLocation(_program, uniform_model_matrix_name)},
            {Uniform::VIEW_MATRIX, glGetUniformLocation(_program, uniform_view_matrix_name)},
            {Uniform::PROJECTION_MATRIX,
             glGetUniformLocation(_program, uniform_projection_matrix_name)},
            {Uniform::LIGHT_POS_VEC3, glGetUniformLocation(_program, uniform_light_position_name)},
            {Uniform::LIGHT_COLOR_VEC3, glGetUniformLocation(_program, uniform_light_color_name)},
            {Uniform::CAMERA_POS_VEC3,
             glGetUniformLocation(_program, uniform_camera_position_name)},
            {Uniform::OBJECT_COLOR_VEC3, glGetUniformLocation(_program, uniform_object_color_name)},
        }};
    }
    return cached_uniform_locations.value();
}

void DefaultShader::use()
{
    glUseProgram(_program);
}

IShader::AttributeLocation DefaultShader::location(Attribute attribute)
{
    if (cached_attribute_locations == std::nullopt)
    {
        (void)attribute_locations();
    }
    return cached_attribute_locations->at(attribute);
}

IShader::UniformLocation DefaultShader::location(Uniform uniform)
{
    if (cached_uniform_locations == std::nullopt)
    {
        (void)uniform_locations();
    }
    return cached_uniform_locations->at(uniform);
}

} // namespace render
