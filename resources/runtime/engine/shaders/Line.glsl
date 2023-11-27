#type vertex
#version 430 core

layout(location = 0) in float a_Identifier;
layout(location = 1) in vec3 a_Position;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out float v_Thickness;

void main()
{
	v_Color = a_Color;
	v_Thickness = a_Thickness;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 430 core

in vec4 v_Color;
in float v_Thickness;

layout (location = 0) out vec4 o_Color;

void main()
{
	o_Color = v_Color;
}