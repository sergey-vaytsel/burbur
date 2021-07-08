#include "TestUtils.h"
#include <catch2/catch.hpp>

#include <Render/Camera.h>

TEST_CASE("Default camera consistence", "[camera]")
{
    render::Camera camera{};
    REQUIRE(glm::normalize(camera.view_direction()) == camera.view_direction());
    REQUIRE(glm::normalize(camera.point_of_view() - camera.position()) == camera.view_direction());
}

TEST_CASE("Camera position, view_direction", "[camera]")
{
    render::Camera camera{};
    SECTION("position")
    {
        const auto point = glm::vec3{1.0f, 2.0f, 3.0f};
        camera.set_position(point);
        REQUIRE(camera.position() == point);
    }
    SECTION("view_direction")
    {
        const auto point = glm::vec3{4.0f, 5.0f, 6.0f};
        camera.set_view_direction(point);
        REQUIRE(camera.view_direction() == glm::normalize(point));
    }
}

TEST_CASE("Camera::look_at", "[camera]")
{
    render::Camera camera{};
    const auto point_of_view = glm::vec3{1.0f, 2.0f, 3.0f};
    camera.look_at(point_of_view);
    REQUIRE(camera.point_of_view() == point_of_view);
    REQUIRE(camera.view_direction() == glm::normalize(point_of_view));

    SECTION("after set_position")
    {
        const auto position = glm::vec3{1.0f};
        camera.set_position(position);
        REQUIRE(camera.point_of_view() == camera.position() + camera.view_direction());

        camera.look_at(point_of_view);
        REQUIRE(camera.point_of_view() == point_of_view);
        REQUIRE(camera.view_direction() == glm::normalize(point_of_view - position));
    }
}
