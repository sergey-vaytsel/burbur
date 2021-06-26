#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>
#include <cstdint>
#include <optional>

#include <glm/glm.hpp>

//! TODO: разнести по файлам

enum class VertexAttributes
{
    VEC3_POSITION,
    VEC3_NORMAL,
    VEC2_TEXCOORD,
    VEC3_RGB_COLOR,
    END
};

enum class Uniforms
{
    MAT4_MVP_MATRIX,
    END
};

class ShaderProgramBuilder;

class Material
{
public:
    Material(
        std::set<VertexAttributes> &&required_vertex_attributes,
        std::set<Uniforms> &&required_uniforms,
        ShaderProgramBuilder &&shader_program);
};
using MaterialLink = std::shared_ptr<Material>;

class Mesh
{
public:
    struct Vertex
    {
        float x;
        float y;
        float z;

        float r;
        float g;
        float b;
    };

    Mesh(std::vector<Vertex> &&, std::vector<unsigned> &&, MaterialLink &&);

private:
    std::vector<Vertex> _vertex_buffer;
    std::vector<unsigned> _index_buffer;
};

class Object
{
public:
    using MatrixToMeshMap = std::unordered_map<glm::mat4, Mesh>;
    Object(MatrixToMeshMap &&);

private:
    MatrixToMeshMap _meshes;
};
