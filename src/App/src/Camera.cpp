#include "Camera.h"

#include <limits>

#include <glm/gtx/vector_angle.hpp>

Camera::Camera(float vertical_fov_rad, float aspect_ratio, float near_plane, float far_plane)
{
    set_projection(vertical_fov_rad, aspect_ratio, near_plane, far_plane);
}

const glm::vec3 &Camera::position()
{
    return _position;
}

void Camera::set_position(const glm::vec3 &position)
{
    _position = position;
    recalculate_view();
}

const glm::vec3 &Camera::point_of_view()
{
    return _point_of_view;
}

const glm::vec3 &Camera::view_direction()
{
    return _view_direction;
}

const glm::vec3 &Camera::up_direction()
{
    return _up_direction;
}

void Camera::set_view_direction(const glm::vec3 &view_direction)
{
    _view_direction = view_direction;
    recalculate_view();
}

void Camera::look_at(const glm::vec3 &point_of_view, const glm::vec3 &up_direction)
{
    _point_of_view = point_of_view;
    _up_direction = up_direction;

    _view_direction = _point_of_view - _position;
    _view_direction /= _view_direction.length();

    recalculate_view();
}

float Camera::vertical_fov()
{
    return _vertical_fov_rad;
}

void Camera::set_vertical_fov(float vertical_fov_rad)
{
    _vertical_fov_rad = vertical_fov_rad;
    recalculate_projection();
}

void Camera::set_aspect_ratio_if_needed(float aspect_ratio)
{
    constexpr auto eps = std::numeric_limits<float>::epsilon();
    if (aspect_ratio > _aspect_ratio + eps || aspect_ratio < _aspect_ratio - eps)
    {
        _aspect_ratio = aspect_ratio;
        recalculate_projection();
    }
}

const glm::mat4 &Camera::view_projection()
{
    if (_need_recalculate_view_projection)
    {
        _view_projection_matrix = _projection_matrix * _view_matrix;
    }
    return _view_projection_matrix;
}

void Camera::set_projection(float vertical_fov_rad, float aspect_ratio, float near_plane, float far_plane)
{
    _vertical_fov_rad = vertical_fov_rad;
    _aspect_ratio = aspect_ratio;
    _near_plane = near_plane;
    _far_plane = far_plane;

    recalculate_projection();
}

void Camera::recalculate_projection()
{
    _projection_matrix = glm::perspective(_vertical_fov_rad, _aspect_ratio, _near_plane, _far_plane);
    need_recalculate_view_projection();
}

void Camera::recalculate_view()
{
    const auto rotate_matrix = g_default_view_direction == _view_direction
                                   ? glm::mat4(1.0f)
                                   : glm::rotate(
                                         glm::mat4(1.0f),
                                         glm::angle(g_default_view_direction, _view_direction),
                                         g_default_view_direction * _view_direction);
    const auto translate_matrix = glm::translate(glm::mat4(1.0f), _position);
    _view_matrix = rotate_matrix * translate_matrix;
    need_recalculate_view_projection();
}

void Camera::need_recalculate_view_projection()
{
    _need_recalculate_view_projection = true;
}
