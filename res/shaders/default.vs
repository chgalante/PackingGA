#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec3 fragment_position;
out vec3 normal;
out vec2 UV;
out vec4 fragment_position_light_space;

uniform mat4 projection_matrix = mat4(1.0f);
uniform mat4 view_matrix = mat4(1.0f);
uniform mat4 model_matrix = mat4(1.0f);
uniform mat4 light_vp;

void main()
{
	fragment_position = vec3(model_matrix * vec4(aPos, 1.0f));
	normal = transpose(inverse(mat3(model_matrix))) * aNorm;
	UV = aUV;
	fragment_position_light_space = light_vp * vec4(fragment_position, 1.0f);
	
	gl_Position =  projection_matrix * view_matrix * vec4(fragment_position, 1.0);
}