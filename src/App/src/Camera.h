#pragma once

#include <glm/glm.hpp>

struct ViewportSize
{
    std::uint16_t width;
    std::uint16_t height;
};

class Camera
{
public:
    Camera() = delete;
    Camera(float vertical_fov, float aspect_ratio_rad, float near_plane, float far_plane);

    const ViewportSize &viewport_size();
    void set_viewport_size(const ViewportSize &size);

    const glm::vec3 &position();
    void set_position(const glm::vec3 &position);

    const glm::vec3 &point_of_view();
    const glm::vec3 &view_direction();
    const glm::vec3 &up_direction();
    void set_view_direction(const glm::vec3 &view_direction);
    void look_at(const glm::vec3 &point_of_view, const glm::vec3 &up_direction = glm::vec3(0.0f, 1.0f, 0.0f));

    float vertical_fov();
    void set_vertical_fov(float vertical_fov);

    glm::mat4 view_projection();
    void set_projection(float vertical_fov, float aspect_ratio_rad, float near_plane, float far_plane);

private:
    ViewportSize _viewport_size;

    glm::mat4 _projection_matrix;
    glm::mat4 _view_matrix;
    glm::mat4 _view_projection_matrix;

    glm::vec3 _position;
    glm::vec3 _up_direction;
    glm::vec3 _point_of_view;
    glm::vec3 _view_direction;

    float _vertical_fov_rad;
    float _aspect_ratio;
    float _near_plane;
    float _far_plane;
};
