#pragma once

#include <optional>
#include <unordered_map>
#include <filesystem>

#include <glad/glad.h>

namespace fs = std::filesystem;

enum class Attribute
{
    POSITION_VEC3,
    NORMAL_VEC3,
    TEXCOORD_VEC2,
    COLOR_VEC3
};

enum class Uniform
{
    MODEL_MATRIX,
    VIEW_MATRIX,
    PROJECTION_MATRIX,
    LIGHT_POS_VEC3,
    LIGHT_COLOR_VEC3,
    CAMERA_POS_VEC3,
    OBJECT_COLOR_VEC3,
};

class IShader
{
public:
    using AttributeLocation = GLuint;
    using AttributeLocations = std::unordered_map<Attribute, AttributeLocation>;

    using UniformLocation = GLint;
    using UniformLocations = std::unordered_map<Uniform, UniformLocation>;

    [[nodiscard]] virtual AttributeLocations attribute_locations() = 0;
    [[nodiscard]] virtual UniformLocations uniform_locations() = 0;
    virtual void use() = 0;

    [[nodiscard]] virtual AttributeLocation location(Attribute) = 0;
    [[nodiscard]] virtual UniformLocation location(Uniform) = 0;

    template <typename Type>
    void set_uniform(Uniform, const Type &);

    virtual ~IShader() = default;
};

class DefaultShader final : public IShader
{
public:
    //! todo
    DefaultShader(const fs::path &vertex_shader_file_path, const fs::path &fragment_shader_file_path);
    ~DefaultShader();

    [[nodiscard]] AttributeLocations attribute_locations();
    [[nodiscard]] UniformLocations uniform_locations();
    void use();

    [[nodiscard]] AttributeLocation location(Attribute);
    [[nodiscard]] UniformLocation location(Uniform);

private:
    GLuint _program = 0;
    std::optional<AttributeLocations> cached_attribute_locations = std::nullopt;
    std::optional<UniformLocations> cached_uniform_locations = std::nullopt;
};

#include <Render/Shader.inl>
