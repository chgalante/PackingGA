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
uniform vec3		camera_position;

uniform vec3		light_position;
uniform vec3        light_direction;
uniform vec4        light_colour;
uniform float       light_cutoff;
uniform bool        light_switch;

void main()
{
	vec3 Normal = normalize(normal);
    vec3 frag_dir, view_dir, reflection_dir;
    float diffuse = 0.0f;
    float specular = 0.0f;
    float ambient = 0.7f;
    float theta;
    /* Spotlight 1 */
    frag_dir = normalize(light_position - fragment_position);
    theta = dot(frag_dir, normalize(-light_direction));    
    if(theta > light_cutoff) 
    {
        if(light_switch)
        {
            view_dir	= normalize(camera_position - fragment_position);
            reflection_dir = reflect(-frag_dir, Normal);
            diffuse += max(dot(Normal, frag_dir), 0.0f);
            specular += 0.1f * pow(max(dot(Normal, reflection_dir), 0.0f), 32.0f);
        }
    }

	if(is_textured)
	{
		vec4 texture_colour = texture(u_texture, UV);
		pixel_colour = (diffuse + specular + ambient) * light_colour * texture_colour;
		pixel_colour = glm::vec4(glm::vec3(pixel_colour), 1.0f);
	}
	else
	{
		pixel_colour = (diffuse + specular + ambient) * light_colour * fragment_colour;
	}
}