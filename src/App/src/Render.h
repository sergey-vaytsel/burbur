#pragma once
#include "Object.h"

class Scene
{
public:
    void add_object(Object &&);
    //! TODO
};

class Render
{
public:
    Render();
    void update_viewport_size(std::uint16_t width, std::uint16_t height);

    double fov = 45.0;
    double z_near = 1.0;
    double z_far = 1000.0;
};
