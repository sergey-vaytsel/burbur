#include <Render/Shader.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace render
{

template <typename Type>
void IShader::set_uniform(Uniform uniform, const Type &value)
{
    if constexpr (std::is_same_v<Type, glm::mat4>)
    {
        glUniformMatrix4fv(location(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<Type, glm::vec3>)
    {
        glUniform3f(location(uniform), value.x, value.y, value.z);
    }
    else
    {
        std::cerr << "set_uniform unsupported type";
    }
}

} // namespace render
