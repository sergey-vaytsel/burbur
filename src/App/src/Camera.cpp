#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera(float vertical_fov_rad, float aspect_ratio, float near_plane, float far_plane)
{
    set_projection(vertical_fov_rad, aspect_ratio, near_plane, far_plane);
}

const ViewportSize &Camera::viewport_size()
{
    return _viewport_size;
}
void Camera::set_viewport_size(const ViewportSize &size)
{
    _viewport_size = size;
}

const glm::vec3 &Camera::position()
{
    return _position;
}
void Camera::set_position(const glm::vec3 &position)
{
    _position = position;
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
}
void Camera::look_at(const glm::vec3 &point_of_view, const glm::vec3 &up_direction)
{
    _point_of_view = point_of_view;
    if (up_direction != glm::vec3(0.0f, 1.0f, 0.0f)) // todo
    {
        _up_direction = up_direction;
    }
    _view_direction = _point_of_view - _position;
    _view_direction /= _view_direction.length();

    const auto default_view_direction = glm::vec3(0.0f, 0.0f, 1.0f);
    _view_matrix = glm::rotate(
                       glm::mat4(1.0f),
                       glm::angle(default_view_direction, _view_direction),
                       default_view_direction * _view_direction) *
                   glm::translate(glm::mat4(1.0f), _position);
}

float Camera::vertical_fov()
{
    return _vertical_fov_rad;
}

void Camera::set_vertical_fov(float vertical_fov_rad)
{
    _vertical_fov_rad = vertical_fov_rad;
}

glm::mat4 Camera::view_projection()
{
    _view_projection_matrix = _projection_matrix * _view_matrix;
    return _view_projection_matrix;
}

void Camera::set_projection(float vertical_fov_rad, float aspect_ratio, float near_plane, float far_plane)
{
    _vertical_fov_rad = vertical_fov_rad;
    _aspect_ratio = aspect_ratio;
    _near_plane = near_plane;
    _far_plane = far_plane;

    _projection_matrix = glm::perspective(vertical_fov_rad, aspect_ratio, near_plane, far_plane);
}