#version 330 core
out vec4 o_vec4_color;

in vec3 v_vec3_normal;
in vec3 v_vec3_position;
  
uniform vec3 u_vec3_light_position; 
uniform vec3 u_vec3_camera_position;
uniform vec3 u_vec3_light_color;
uniform vec3 u_vec3_object_color;

void main()
{
    float ambient_strength = 0.1f;
    vec3 ambient_light_color = ambient_strength * u_vec3_light_color;
  	
    vec3 normal = normalize(v_vec3_normal);
    vec3 light_direction = normalize(u_vec3_light_position - v_vec3_position);
    float diffuse_strength = max(dot(normal, light_direction), 0.0);
    vec3 diffuse_light_color = diffuse_strength * u_vec3_light_color;
    
    float specular_strength_factor = 0.5f;
    vec3 view_direction = normalize(u_vec3_camera_position - v_vec3_position);
    vec3 reflect_direction = reflect(-light_direction, normal);  
    float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular_light_color = specular_strength_factor * specular_strength * u_vec3_light_color;  
        
    vec3 result_color = (ambient_light_color + diffuse_light_color + specular_light_color) * u_vec3_object_color;
    o_vec4_color = vec4(result_color, 1.0f);
} 