#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;


uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color =  a_Color;
	gl_Position = u_ViewProjection *u_Transform* vec4(a_Position,1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;


in vec4 v_Color;
in vec2 v_TexCoord;


void main()
{
	FragColor = vec4(v_TexCoord,0.f,1.f);
	
}

