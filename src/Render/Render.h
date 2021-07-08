#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <Render/Scene.h>

namespace render
{

class Renderer final
{
public:
    Renderer(GLADloadproc glad_load_procedure);
    void update_viewport_size_if_needed(std::uint16_t width, std::uint16_t height);

    void bind(const Node &node);
    void draw(const Node &node);
    void draw(const Scene &scene);

private:
    std::uint16_t _width = 0;
    std::uint16_t _height = 0;
};

} // namespace render
