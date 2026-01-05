#version 430
precision mediump float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform sampler2D u_galaxy;
uniform sampler2D u_blur;

void main()
{
	vec3 stars = texture(u_galaxy, v_texcoord).rgb;
	//vec3 glow = texture(u_blur, v_texcoord).rgb * 0.2;

	frag_color = vec4(stars /*+ glow*/, 1.);
}
