#pragma once

#include <optional>
#include <string>

class ShaderProgramBuilder
{
public:
    ShaderProgramBuilder &add_fragment_shader(std::string &&text);
    ShaderProgramBuilder &add_vertex_shader(std::string &&text);

    std::uint32_t build();

private:
    std::optional<std::string> _vertex_shader_text = std::nullopt;
    std::optional<std::string> _fragment_shader_text = std::nullopt;
};