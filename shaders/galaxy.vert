#version 430
precision mediump float;


uniform vec4 u_color_positive_mass;
uniform vec4 u_color_negative_mass;
uniform mat4 u_mvp;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec4 a_speed;
layout (location = 2) in float a_mass;

out float v_brightness;
out vec4 v_color;

void main()
{
	v_brightness = length(vec3(a_speed));
	v_color = a_mass >= 0 ? u_color_positive_mass : u_color_negative_mass;
	gl_Position = u_mvp * vec4(a_position.xyz, 1.);
}
