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

uniform vec3		light_position_one;
uniform vec3        light_direction_one;
uniform vec4        light_colour_one;
uniform float       light_cutoff_one;
uniform bool        light_switch_one;

uniform vec3		light_position_two;
uniform vec3        light_direction_two;
uniform vec4        light_colour_two;
uniform float       light_cutoff_two;
uniform bool        light_switch_two;

uniform vec3		light_position_three;
uniform vec3        light_direction_three;
uniform vec4        light_colour_three;
uniform float       light_cutoff_three;
uniform bool        light_switch_three;

void main()
{
	vec3 Normal = normalize(normal);
    vec3 frag_dir, view_dir, reflection_dir;
    float diffuse = 0.0f;
    float specular = 0.0f;
    float ambient = 0.1f;
    vec4 light_colour = vec4(1.0f);
    float theta;
    int num_light_sources;
    /* Spotlight 1 */
    frag_dir = normalize(light_position_one - fragment_position);
    theta = dot(frag_dir, normalize(-light_direction_one));    
    if(theta > light_cutoff_one) 
    {
        if(light_switch_one)
        {
            view_dir	= normalize(camera_position - fragment_position);
            reflection_dir = reflect(-frag_dir, Normal);
            diffuse += max(dot(Normal, frag_dir), 0.0f);
            specular += 0.1f * pow(max(dot(Normal, reflection_dir), 0.0f), 32.0f);
            light_colour *= light_colour_one;
            num_light_sources++;
        }
    }
    /* Spotlight 2 */
    frag_dir = normalize(light_position_two - fragment_position);
    theta = dot(frag_dir, normalize(-light_direction_two));    
    if(theta > light_cutoff_two) 
    {
        if(light_switch_two)
        {
            view_dir	= normalize(camera_position - fragment_position);
            reflection_dir = reflect(-frag_dir, Normal);
            diffuse += max(dot(Normal, frag_dir), 0.0f);
            specular += 0.1f * pow(max(dot(Normal, reflection_dir), 0.0f), 32.0f);
            light_colour *= light_colour_two;
            num_light_sources++;
        }
    }
    /* Spotlight 3 */
    frag_dir = normalize(light_position_three - fragment_position);
    theta = dot(frag_dir, normalize(-light_direction_three));    
    if(theta > light_cutoff_three) 
    {
        if(light_switch_three)
        {
            view_dir	= normalize(camera_position - fragment_position);
            reflection_dir = reflect(-frag_dir, Normal);
            diffuse += max(dot(Normal, frag_dir), 0.0f);
            specular += 0.1f * pow(max(dot(Normal, reflection_dir), 0.0f), 32.0f);
            light_colour *= light_colour_three;
            num_light_sources++;
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