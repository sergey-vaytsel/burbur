#pragma once

#include <numbers>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace render
{

const auto g_default_view_direction = glm::vec3(0.0f, 0.0f, 1.0f);
const auto g_default_up_direction = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
public:
    Camera() = default;

    void set_position(const glm::vec3 &position);
    void set_view_direction(const glm::vec3 &view_direction);
    void look_at(const glm::vec3 &point_of_view,
                 const glm::vec3 &up_direction = g_default_up_direction);
    void set_aspect_ratio_if_needed(float aspect_ratio);

    [[nodiscard]] const glm::mat4 &view();
    [[nodiscard]] const glm::mat4 &projection();

    [[nodiscard]] const glm::vec3 &position();
    [[nodiscard]] const glm::vec3 &point_of_view();
    [[nodiscard]] const glm::vec3 &view_direction();

private:
    void recalculate_projection();
    void recalculate_view();

    glm::vec3 _position{0.0f};
    glm::vec3 _up_direction = glm::vec3{0.0f, 1.0f, 0.0f};
    glm::vec3 _point_of_view = g_default_view_direction;
    glm::vec3 _view_direction = g_default_view_direction;

    float _vertical_fov_rad = std::numbers::pi_v<float> / 4;
    float _aspect_ratio = 1.333f;
    float _near_plane = 1.0f;
    float _far_plane = 1000.0f;

    glm::mat4 _projection_matrix =
        glm::perspective(_vertical_fov_rad, _aspect_ratio, _near_plane, _far_plane);
    glm::mat4 _view_matrix{1.0f};
};

} // namespace render
