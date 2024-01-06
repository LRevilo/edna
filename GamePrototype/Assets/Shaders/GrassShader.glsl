#type vertex
#version 410 core
// shader to print out bits as a 8x8 grid

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

#type geometry
#version 410 core


#type fragment
#version 410 core

layout(location = 0) out vec4 FragColor;


in vec4 v_Color;
in vec2 v_TexCoord;
uniform uint u_LowerBits;
uniform uint u_UpperBits;

void main()
{
	vec2 texel = min(floor((v_TexCoord)*8),7);
	uvec2 bits = uvec2(texel);

	uint X = 7-bits.x;
	uint Y = bits.y;
	uint N = X + 8*Y;
	uint M = (N<32) ? N : N - 32;

	// A, B are 2 32 bit ints representing int C (equivalent 64 bits)

	// outline
	//uint A = 0x818181FF;
	//uint B = 0xFF818181;

	uint A = u_LowerBits;
	uint B = u_UpperBits;

	uint C = (N<32) ? A : B;

	float Result = float(C>>M&1);



	float Border = ( Y==7||X==0||Y==0||X==7) ? 1.0 : 0.0;




	FragColor = vec4(Result,Result,Result,1.f);
	