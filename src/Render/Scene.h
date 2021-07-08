#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include <glm/glm.hpp>

#include <Render/Shader.h>

namespace render
{

class Scene;
class Node
{
public:
    template <typename VertexBuffer>
    struct VBO_Binding
    {
        Attribute attribute;
        std::reference_wrapper<VertexBuffer> vertex_buffer_ref;
        ptrdiff_t stride;
    };

    template <typename VertexBuffer>
    [[nodiscard]] std::vector<VertexBuffer> vertex_buffers();
    template <typename VertexBuffer>
    [[nodiscard]] std::vector<VBO_Binding<VertexBuffer>> VAO_Bindings();

    [[nodiscard]] const glm::mat4 &transformation();
    [[nodiscard]] const glm::vec3 &position();
    void set_position(glm::vec3 position);
    void set_scales(glm::vec3 scales);
    void set_rotations(glm::vec3 rotations);

    void add_child(Node node);
    [[nodiscard]] const Node &parent();
    [[nodiscard]] const std::vector<std::reference_wrapper<Node>> &children();

private:
    std::reference_wrapper<Scene> _scene;
    std::vector<std::reference_wrapper<Node>> children_refs;
};

class Scene
{
    friend class Node;
    const Node &add_child(Node node);

public:
    [[nodiscard]] const std::optional<std::reference_wrapper<Node>> root();

private:
    std::vector<Node> _nodes;
};

} // namespace render
