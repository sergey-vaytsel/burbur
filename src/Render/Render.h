#pragma once

#include <cstdint>
#include <glad/glad.h>

class Render
{
public:
    Render(GLADloadproc glad_load_procedure);
    void update_viewport_size_if_needed(std::uint16_t width, std::uint16_t height);

    double fov = 45.0;
    double z_near = 1.0;
    double z_far = 1000.0;

private:
    std::uint16_t _width = 0;
    std::uint16_t _height = 0;
};
