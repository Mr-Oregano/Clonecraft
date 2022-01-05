#version 460 core

layout(location = 0) in vec4 i_Pos;
layout(location = 1) in vec2 i_Tex;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec4 v_VertColor;
out vec2 v_TexCoords;

void main()
{
	gl_Position = u_ViewProj * u_Model * i_Pos;
	v_VertColor = i_Pos;
	v_TexCoords = i_Tex;
}