#type vertex
#version 430 core

layout(location = 0) in float a_Identifier;
layout(location = 1) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out float v_Identifier;
out vec2 v_TexCoord;
out vec3 v_Position;

void main()
{
	v_Identifier = a_Identifier;
	v_TexCoord = a_TexCoord;
	v_Position = a_Position;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type geometry
#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in float v_Identifier[];
in vec2 v_TexCoord[];
in vec3 v_Position[];

out float f_Identifier;
out vec3 f_Position;
out vec2 f_TexCoord;
out vec3 f_BarryCoord;

void main()
{
	for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
		f_Identifier = v_Identifier[i];
	 	f_TexCoord = v_TexCoord[i];
		f_Position = v_Position[i];
        f_BarryCoord = vec3(i == 0, i == 1, i == 2);

        EmitVertex();
    }

    EndPrimitive();
}

#type fragment
#version 430 core

in float f_Identifier;
in vec2 f_TexCoord;
in vec3 f_BarryCoord;
in vec3 f_Position;

layout(location = 0) out vec4 o_Color;

struct Material
{
	vec4 m_Color;
	float m_TilingFactor;
	float m_TextureID;
	float m_Highlight;
	float _pad;
};

layout(std430, binding = 1) buffer MaterialSSBO
{
	Material u_Materials[];
};

struct Light
{
	vec3 position;
	float intensity;
	vec4 color;
};

layout(std430, binding = 2) buffer LightSSBO
{
	Light u_Lights[];
};

uniform vec4 u_AmbientColor;
uniform float u_AmbientIntensity;

uniform sampler2D u_Textures[32];

void main()
{
	int id = int(f_Identifier);

	const float wireDistance = 0.025f; 

	if((f_BarryCoord.x < wireDistance || f_BarryCoord.z < wireDistance) && u_Materials[id].m_Highlight > 0.5)
	{
		o_Color = vec4(0.9372, 0.5372, 0.2509, 1.0);
	}
	else
	{
		//o_Color = vec4(gl_FragDepth, 0.0, 0.0, 1.0);
		vec4 color = texture(u_Textures[int(u_Materials[id].m_TextureID)], f_TexCoord * u_Materials[id].m_TilingFactor) * u_Materials[id].m_Color;
		//o_Color = color;

		for(int i = 0; i < 1; i++)
		{
			float distance = distance(u_Lights[i].position, f_Position);

			o_Color += color * (1.0 / (distance + 0.01)) * u_Lights[i].intensity;
		}

		o_Color += color * u_AmbientColor * u_AmbientIntensity;
	}
}