#include <Render/Camera.h>

#include <limits>

#include <glm/gtx/vector_angle.hpp>

void Camera::set_position(const glm::vec3 &position)
{
    _position = position;
    _point_of_view = _position + _view_direction;
    recalculate_view();
}

void Camera::set_view_direction(const glm::vec3 &view_direction)
{
    _view_direction = glm::normalize(view_direction);
    _point_of_view = _position + _view_direction;
    recalculate_view();
}

void Camera::look_at(const glm::vec3 &point_of_view, const glm::vec3 &up_direction)
{
    _point_of_view = point_of_view;
    _up_direction = up_direction;
    _view_direction = glm::normalize(_point_of_view - _position);
    recalculate_view();
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

const glm::mat4 &Camera::view()
{
    return _view_matrix;
}

const glm::mat4 &Camera::projection()
{
    return _projection_matrix;
}

const glm::vec3 &Camera::position()
{
    return _position;
}

const glm::vec3 &Camera::point_of_view()
{
    return _point_of_view;
}

const glm::vec3 &Camera::view_direction()
{
    return _view_direction;
}

void Camera::recalculate_projection()
{
    _projection_matrix =
        glm::perspective(_vertical_fov_rad, _aspect_ratio, _near_plane, _far_plane);
}

void Camera::recalculate_view()
{
    _view_matrix = glm::lookAt(_position, _point_of_view, _up_direction);
}
