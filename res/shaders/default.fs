#version 330 core

in vec3		fragment_position;
in vec3		normal;
in vec2 	UV;
in vec4		fragment_position_light_space;
out vec4 	pixel_colour;

uniform sampler2D 	u_texture;
uniform sampler2D	shadow_map;
uniform bool		is_textured;
uniform vec4 		fragment_colour = vec4(1.0f);
uniform float 		ambient_light_intensity = 0.1f;
uniform vec3		light_position_one;
uniform vec3		camera_position;

float CalculateShadow()
{
	vec3 ndc = fragment_position_light_space.xyz / fragment_position_light_space.w;
    // transform to [0,1] range
    ndc = ndc * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragment_position_light_space as coords)
    float closest_depth = texture(shadow_map, ndc.xy).r;
    // get depth of current fragment from light's perspective
    float current_depth = ndc.z;
    // check whether current frag pos is in shadow
    return ((current_depth) < closest_depth) ? 1.0 : 0.0;
}

void main()
{
	vec3 	Normal = normalize(normal);
	vec3 	light_direction = normalize(light_position_one - fragment_position);
	vec3	view_direction	= normalize(camera_position - fragment_position);
	vec3	reflection_direction = reflect(-light_direction, Normal);
	float 	diffuse_light_intensity = max(dot(Normal, light_direction), 0.0f);
	float	specular_light_intensity = 0.2f * pow(max(dot(Normal, reflection_direction), 0.0f), 32.0f);
	//float	shadow = CalculateShadow();
	float	shadow = 1.0;

	if(is_textured)
	{
		vec4 	texture_colour = texture(u_texture, UV);
		pixel_colour = (shadow * (diffuse_light_intensity + specular_light_intensity) + ambient_light_intensity) * texture_colour;
		pixel_colour = glm::vec4(glm::vec3(pixel_colour), 1.0f);
	}
	else
	{
		pixel_colour = (shadow * (diffuse_light_intensity + specular_light_intensity) + ambient_light_intensity) * fragment_colour;
	}
}