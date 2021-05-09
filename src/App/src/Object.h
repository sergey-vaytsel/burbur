#include <vector>
#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>

//! TODO: разнести по файлам

class Material {
public:
    enum class VertexAttributes {
        VEC3_POSITION,
        VEC3_RGB_COLOR,
    };
    enum class Uniforms {
        MAT4_MVP_MATRIX,
    };
    //! TODO: интерфейс
}

using MaterialLink = std::shared_ptr<Material>;

class Mesh {
public:
    struct Vertex {
        float x;
        float y;
        float z;

        float r;
        float g;
        float b;
    };
    Mesh(std::vector<Vertex> &&, MaterialLink &&);
private:
    std::vector<Vertex> vertex_buffer;
};

class Object {
public:
    using MatrixToMeshMap = std::unordered_map<glm::mat4, Mesh>;
    Object(MatrixToMeshMap &&);
private:
    MatrixToMeshMap meshes;
};
