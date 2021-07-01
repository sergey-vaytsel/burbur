#version 330 core
layout (location = 0) in vec3 a_vec3_position;
layout (location = 1) in vec3 a_vec3_normal;

out vec3 v_vec3_normal;
out vec3 v_vec3_position;

uniform mat4 u_mat4_model;
uniform mat4 u_mat4_view;
uniform mat4 u_mat4_projection;

void main()
{
    mat4 mat4_mvp = u_mat4_projection * u_mat4_view *  u_mat4_model;
    gl_Position = mat4_mvp * vec4(a_vec3_position, 1.0f);
    v_vec3_position = vec3(u_mat4_model * vec4(a_vec3_position, 1.0f));
    v_vec3_normal = mat3(transpose(inverse(u_mat4_model))) * a_vec3_normal;  
} 