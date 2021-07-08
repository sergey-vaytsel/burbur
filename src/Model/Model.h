#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <type_traits>
#include <vector>

#include <glm/glm.hpp>

namespace fs = std::filesystem;

namespace model
{

struct ModelImportSettings final
{
    /** Triangulates all faces of all meshes.
     */
    bool triangulate = true;
    /** Generates normals for all faces of all meshes.
     * This is ignored if normals are already there.
     */
    bool gen_normals = true;
    /** For rendering, you can simply render all meshes in order.
     * Animations was removed.
     */
    bool pretransform_vertices = false;
    /** Reorders triangles for better vertex cache locality.
     */
    bool improve_cache_locality = false;
    bool join_identical_vertices = false;
};

struct Mesh
{
    std::span<glm::vec3> vertices;
    std::span<glm::vec3> normals;
    std::span<std::uint32_t> indices;
    //! /todo
    // static constexpr std::uint8_t max_number_of_color_sets = 8;
    // static constexpr std::uint8_t max_number_of_texture_coord = 8;
    // std::span<glm::vec4> colors[max_number_of_color_sets];
    // std::span<glm::vec3> texture_coords[max_number_of_texture_coord];
    // std::span<std::uint8_t> num_uv_components[max_number_of_texture_coord];
};

struct Texture
{
    struct TexelARGB8888
    {
        unsigned char b, g, r, a;
    };

    std::uint32_t width;
    std::uint32_t height;
    std::vector<TexelARGB8888> data;
};

struct Node
{
    std::optional<glm::mat4> transformation;
    std::span<Node> children;
    std::vector<std::uint32_t> mesh_indices;
};

class Model final
{
public:
    Model(const fs::path &model_path, const ModelImportSettings &settings = {});

    [[nodiscard]] const std::optional<std::reference_wrapper<Node>> root();
    [[nodiscard]] const std::span<Mesh> meshes();
    [[nodiscard]] const std::span<Texture> textures();

    [[nodiscard]] bool has_meshes();
    [[nodiscard]] bool has_textures();

private:
    std::vector<glm::vec3> _vertices = {};
    std::vector<glm::vec3> _normals = {};
    std::vector<std::uint32_t> _indices = {};
    std::vector<Mesh> _meshes = {};

    std::vector<Texture> _textures = {};

    /** Ноды дерева, отсортированные по глубине и по родителю.
     * Дети каждой ноды расположены последовательно.
     * children каждой ноды указывает на подмножество нод из этого вектора.
     */
    std::vector<Node> _nodes = {};
};

} // namespace model
