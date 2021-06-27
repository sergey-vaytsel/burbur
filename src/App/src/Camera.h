#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto g_default_view_direction = glm::vec3(0.0f, 0.0f, 1.0f);

struct ViewportSize
{
    std::uint16_t width;
    std::uint16_t height;
};

class Camera
{
public:
    Camera() = default;
    Camera(float vertical_fov, float aspect_ratio_rad, float near_plane, float far_plane);

    const glm::vec3 &position();
    void set_position(const glm::vec3 &position);

    const glm::vec3 &point_of_view();
    const glm::vec3 &view_direction();
    const glm::vec3 &up_direction();
    void set_view_direction(const glm::vec3 &view_direction);
    void look_at(const glm::vec3 &point_of_view, const glm::vec3 &up_direction = glm::vec3(0.0f, 1.0f, 0.0f));

    float vertical_fov();
    void set_vertical_fov(float vertical_fov);

    void set_aspect_ratio_if_needed(float aspect_ratio);

    const glm::mat4 &view_projection();
    void set_projection(float vertical_fov, float aspect_ratio_rad, float near_plane, float far_plane);

private:
    void recalculate_projection();
    void recalculate_view();
    void need_recalculate_view_projection();

    glm::vec3 _position{0.0f};
    glm::vec3 _up_direction = glm::vec3{0.0f, 1.0f, 0.0f};
    glm::vec3 _point_of_view{0.0f};
    glm::vec3 _view_direction = g_default_view_direction;

    float _vertical_fov_rad = glm::pi<float>() * 0.25f;
    float _aspect_ratio = 1.333f;
    float _near_plane = 1.0f;
    float _far_plane = 1000.0f;

    glm::mat4 _projection_matrix = glm::perspective(_vertical_fov_rad, _aspect_ratio, _near_plane, _far_plane);
    glm::mat4 _view_matrix{1.0f};
    glm::mat4 _view_projection_matrix = _projection_matrix * _view_matrix;
    bool _need_recalculate_view_projection = false;
};
