#include <Model/Model.h>

#include <cstddef>
#include <type_traits>
#include <iostream>
#include <numeric>
#include <deque>
#include <functional>

#include <fmt/format.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace model
{

    inline namespace
    {
        auto import_settings_to_assimp_flags(const ModelImportSettings &settings) -> aiPostProcessSteps
        {
            auto flags = 0x0u;
            const auto add = [&flags](const aiPostProcessSteps &flag)
            {
                flags = flags | flag;
            };

            if (settings.triangulate)
            {
                add(aiProcess_Triangulate);
            }
            if (settings.gen_normals)
            {
                add(aiProcess_GenNormals);
            }
            if (settings.pretransform_vertices)
            {
                add(aiProcess_PreTransformVertices);
            }
            if (settings.improve_cache_locality)
            {
                add(aiProcess_ImproveCacheLocality);
            }
            if (settings.join_identical_vertices)
            {
                add(aiProcess_JoinIdenticalVertices);
            }
            return static_cast<aiPostProcessSteps>(flags);
        }

        using Meshes = std::vector<Mesh>;
        using Vertices = std::vector<glm::vec3>;
        using Normals = std::vector<glm::vec3>;
        using Indices = std::vector<std::uint32_t>;

        std::tuple<Vertices, Normals, Indices, Meshes> make_meshes(const aiScene *const scene)
        {
            static_assert(sizeof(aiVector3D) == sizeof(glm::vec3));
            static_assert(offsetof(aiVector3D, aiVector3D::x) == offsetof(glm::vec3, glm::vec3::x));
            static_assert(offsetof(aiVector3D, aiVector3D::y) == offsetof(glm::vec3, glm::vec3::y));
            static_assert(offsetof(aiVector3D, aiVector3D::z) == offsetof(glm::vec3, glm::vec3::z));

            static_assert(std::is_same_v<
                          std::remove_pointer_t<decltype(aiFace::mIndices)>,
                          decltype(Mesh::indices)::value_type>);

            const auto scene_meshes = std::span{scene->mMeshes, scene->mNumMeshes};
            const auto [vertices_count, indices_count] = std::accumulate(
                scene_meshes.begin(),
                scene_meshes.end(),
                std::tuple{0u, 0u},
                [](const auto tuple_acc, const auto mesh)
                {
                    const auto [vertices_acc, indices_acc] = tuple_acc;
                    const auto faces = std::span{mesh->mFaces, mesh->mNumFaces};
                    return std::tuple{vertices_acc + mesh->mNumVertices,
                                      std::accumulate(
                                          faces.begin(),
                                          faces.end(),
                                          indices_acc,
                                          [](const auto face_indices_acc, const auto &face)
                                          { return face_indices_acc + face.mNumIndices; })};
                });

            Meshes meshes;
            Vertices vertices;
            Normals normals;
            Indices indices;
            meshes.reserve(scene_meshes.size());
            vertices.reserve(vertices_count);
            normals.reserve(vertices_count);
            indices.reserve(indices_count);

            std::vector<ptrdiff_t> first_vertices_indexes;
            std::vector<ptrdiff_t> first_normals_indexes;
            std::vector<ptrdiff_t> first_indices_indexes;
            first_vertices_indexes.reserve(scene_meshes.size());
            first_normals_indexes.reserve(scene_meshes.size());
            first_indices_indexes.reserve(scene_meshes.size());

            for (const auto mesh : scene_meshes)
            {
                first_vertices_indexes.push_back(static_cast<ptrdiff_t>(vertices.size()));
                const auto mesh_vertices = std::span{
                    reinterpret_cast<glm::vec3 *>(mesh->mVertices),
                    mesh->mNumVertices};
                vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());

                first_normals_indexes.push_back(static_cast<ptrdiff_t>(normals.size()));
                if (mesh->mNormals != nullptr)
                {
                    const auto mesh_normals = std::span{
                        reinterpret_cast<glm::vec3 *>(mesh->mNormals),
                        mesh->mNumVertices};
                    normals.insert(normals.end(), mesh_normals.begin(), mesh_normals.end());
                }

                first_indices_indexes.push_back(static_cast<ptrdiff_t>(indices.size()));
                for (const auto face : std::span{mesh->mFaces, mesh->mNumFaces})
                {
                    const auto face_indices = std::span(face.mIndices, face.mNumIndices);
                    indices.insert(indices.end(), face_indices.begin(), face_indices.end());
                }
            }
            for (size_t i = 0; i < scene_meshes.size(); i++)
            {
                const auto mesh = scene_meshes[i];
                const auto first_vertices_index = first_vertices_indexes[i];
                const auto first_normals_index = first_normals_indexes[i];
                const auto first_indices_index = first_indices_indexes[i];

                meshes.emplace_back(
                    std::span{vertices.begin() + first_vertices_index, vertices.end()},
                    mesh->mNormals == nullptr
                        ? std::span<glm::vec3>{}
                        : std::span{normals.begin() + first_normals_index, normals.end()},
                    std::span{indices.begin() + first_indices_index, indices.end()});
            }
            if (normals.size() != 0 && normals.size() != vertices.size())
            {
                std::cerr << "unsupported model format. all meshes must have vertex or no one. fallback - remove normals";
                normals.clear();
            }
            return {
                std::move(vertices),
                std::move(normals),
                std::move(indices),
                std::move(meshes)};
        }

        using Textures = std::vector<Texture>;

        Textures make_textures(const aiScene *const scene)
        {
            using Texel = Texture::TexelARGB8888;
            static_assert(sizeof(aiTexel) == sizeof(Texel));
            static_assert(offsetof(aiTexel, aiTexel::b) == offsetof(Texel, Texel::b));
            static_assert(offsetof(aiTexel, aiTexel::g) == offsetof(Texel, Texel::g));
            static_assert(offsetof(aiTexel, aiTexel::r) == offsetof(Texel, Texel::r));
            static_assert(offsetof(aiTexel, aiTexel::a) == offsetof(Texel, Texel::a));

            Textures textures;
            textures.reserve(scene->mNumTextures);
            for (const auto texture : std::span{scene->mTextures, scene->mNumTextures})
            {
                auto width = texture->mWidth;
                auto height = texture->mHeight;
                const auto texture_data = std::span{reinterpret_cast<Texel *>(texture->pcData), width * height};
                textures.emplace_back(std::move(width),
                                      std::move(height),
                                      std::vector<Texel>{texture_data.begin(), texture_data.end()});
            }
            return textures;
        }

        std::optional<glm::mat4> node_matrix(const aiNode *const assimp_node)
        {
            auto &m = assimp_node->mTransformation;
            if (m.IsIdentity())
            {
                return std::nullopt;
            }
            auto transform = glm::mat4{
                m.a1, m.a2, m.a3, m.a4,
                m.b1, m.b2, m.b3, m.b4,
                m.c1, m.c2, m.c3, m.c4,
                m.d1, m.d2, m.d3, m.d4};
            return transform;
        }

        using Nodes = std::vector<Node>;
        using NodeVisitor = std::function<void(const aiNode *const)>;

        void traverse(NodeVisitor &&visitor, aiNode *root_node)
        {
            if (root_node == nullptr)
            {
                return;
            }
            std::deque<aiNode *> node_deque;
            node_deque.push_back(root_node);
            for (auto i = 0u; i < node_deque.size(); i++)
            {
                const auto current_node = node_deque.at(i);
                visitor(current_node);
                for (auto child : std::span{current_node->mChildren, current_node->mNumChildren})
                {
                    node_deque.push_back(child);
                }
            }
        }

        Nodes make_nodes(const aiScene *scene)
        {
            static_assert(std::is_same_v<
                          decltype(Node::mesh_indices)::value_type,
                          std::remove_pointer_t<decltype(aiNode::mMeshes)>>);

            Nodes nodes;
            // сначала запомним число детей каждой ноды
            // после заполнения списка нод отдельным проходом проставим ссылки на детей
            std::vector<size_t> num_children;
            traverse([&nodes, &num_children](const aiNode *const node)
                     {
                         const auto meshes = std::span{node->mMeshes, node->mNumMeshes};
                         nodes.emplace_back(node_matrix(node),
                                            std::span<Node>{},
                                            std::vector<std::uint32_t>{meshes.begin(), meshes.end()});
                         num_children.push_back(node->mNumChildren);
                     },
                     scene->mRootNode);
            nodes.shrink_to_fit();

            ptrdiff_t nodes_offset = 1; // root node
            for (auto i = 0u; i < nodes.size(); i++)
            {
                auto &node = nodes[i];
                const auto &node_num_children = num_children[i];
                node.children = std::span{nodes.begin() + nodes_offset, node_num_children};
                nodes_offset += static_cast<ptrdiff_t>(node_num_children);
            }

            return nodes;
        }
    }

    Model::Model(const fs::path &model_path, const ModelImportSettings &settings)
    {
        Assimp::Importer model_importer;
        const auto scene = model_importer.ReadFile(
            model_path.string(),
            import_settings_to_assimp_flags(settings));

        if (scene == nullptr)
        {
            throw std::runtime_error(fmt::format(
                "Import model '{}' error: {}",
                model_path.string(),
                model_importer.GetErrorString()));
        }
        std::tie(_vertices, _normals, _indices, _meshes) = make_meshes(scene);
        _textures = make_textures(scene);
        _nodes = make_nodes(scene);
    }

    const std::optional<std::reference_wrapper<Node>> Model::root()
    {
        if (_nodes.empty())
        {
            return std::nullopt;
        }
        return _nodes[0];
    }

    const std::span<Mesh> Model::meshes()
    {
        return _meshes;
    }

    const std::span<Texture> Model::textures()
    {
        return _textures;
    }

    bool Model::has_meshes()
    {
        return not _meshes.empty();
    }

    bool Model::has_textures()
    {
        return not _textures.empty();
    }

} // namespace model
