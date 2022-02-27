#type vertex
#version 330 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 f_TexCoords;

void main()
{
	f_TexCoords = v_TexCoords;

	gl_Position = u_ViewProjection * u_Transform * vec4(v_Position, 1.0);
	
}

#type fragment
#version 330 core

in vec2 f_TexCoords;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, f_TexCoords);
}