#pragma once

#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

std::ostream &operator<<(std::ostream &os, const glm::vec3 &value)
{
    os << glm::to_string(value);
    return os;
}
