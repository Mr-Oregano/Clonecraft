#version 460 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Sampler;

in vec4 v_VertColor;
in vec2 v_TexCoords;

void main()
{
	o_Color = v_VertColor + vec4(0.5);
	o_Color = texture(u_Sampler, v_TexCoords);
}