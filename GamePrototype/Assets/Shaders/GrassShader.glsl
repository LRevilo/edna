#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	float TexIndex;
    vec4 Color;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_LightSpace;
uniform float u_TexIndex;

void main()
{
	//v_TexCoord = a_TexCoord;
	//v_Color =  a_Color;
	//gl_Position = u_ViewProjection *u_Transform* vec4(a_Position,1.0);


    vs_out.FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(u_Transform))) * a_Normal;
    vs_out.TexCoords = a_TexCoord;
    vs_out.TexIndex = u_TexIndex;
    vs_out.Color = a_Color;
    vs_out.FragPosLightSpace = u_LightSpace * vec4(vs_out.FragPos, 1.0);
    gl_Position = u_Transform * vec4(a_Position, 1.0);
}

///////////////////////////////////////////////////////////////////////////////////////
#type geometry
#version 410 core

layout (triangles, invocations = 1) in;
layout (triangle_strip, max_vertices = 39 ) out;



in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	float TexIndex;
    vec4 Color;
    vec4 FragPosLightSpace;
} gs_in[];

out GS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	float TexIndex;
    vec4 Color;
    vec4 FragPosLightSpace;
} gs_out;

uniform uint u_LowerBits;
uniform uint u_UpperBits;


const float MAGNITUDE = 0.4;

uniform mat4 u_ViewProjection;

struct Primitive {

    vec4 AveragePos;
    vec4 AverageNormal;
    vec4 AverageColor;
};


void GenerateLine(int index)
{
    gl_Position = u_ViewProjection * gl_in[index].gl_Position;
  
    EmitVertex();
    gl_Position = u_ViewProjection * (gl_in[index].gl_Position + 
                                vec4(gs_in[index].Normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

Primitive PassBaseMesh()
{
    
    //gl_InstanceIndex

    Primitive p;
     
     p.AveragePos = vec4(0);
     p.AverageNormal = vec4(0);
     p.AverageColor = vec4(0);

    for (int i = 0; i <gl_in.length(); i++)
    {
        gl_Position = u_ViewProjection * (gl_in[i].gl_Position);
        gs_out.Color = gs_in[i].Color+ vec4(0,0,0.5,0); 
        gs_out.TexCoords = gs_in[i].TexCoords;
        p.AverageColor += gs_in[i].Color;
        p.AveragePos += gl_in[i].gl_Position;
        p.AverageNormal += vec4(gs_in[i].Normal,0);
        EmitVertex();
    }
    EndPrimitive();
    
    //avg_pos -= (gl_in[0].gl_Position + gl_in[2].gl_Position);
    p.AverageColor /= gl_in.length();
    p.AveragePos /= gl_in.length();
    p.AverageNormal /= gl_in.length();

    return p;

}

void AddGrass(vec4 pos, vec4 normal, vec4 color)
{

    float r = dot(sin(16*color+42069.42069*(color*color)),color+cos(1.618*color*color));
    float t = fract(5*fract(2*r*r-1.0) + 11*sin(4*r) + 3*r);
    float q = fract(t*t + r + cos(3*r*t));

    t = 2.0*t - 1.0;
    q = 2.0*q - 1.0;

    float ds = 1.0/4.0;
    float dw = ds/8.0;

    gs_out.Color = vec4(0,0.2,0,1); 
    gl_Position = u_ViewProjection * (pos + vec4(t*ds-dw,q*ds,0,0));
    EmitVertex();
    gs_out.Color = vec4(0,0.7,0,1); 
    gl_Position = u_ViewProjection * (pos + vec4(t*ds,q*ds+2*dw,0.4,0));
    EmitVertex();
    gs_out.Color = vec4(0,0.2,0,1); 
    gl_Position = u_ViewProjection * (pos + vec4(t*ds+dw,q*ds,0,0));
    EmitVertex();
    EndPrimitive();
}

void main()
{
    Primitive p = PassBaseMesh();

    vec4 c = p.AverageColor;

    for (int i = 0; i < 12; i++)
    {
    AddGrass(p.AveragePos,p.AverageNormal,c);
    c = 3.44*(1.0 - c)*c;
    }

    
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
#type fragment
#version 410 core
#include "../EDNA/src/EDNA/Shaders/PhongLighting.glsl"

layout(location = 0) out vec4 FragColor;

in GS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	float TexIndex;
    vec4 Color;
    vec4 FragPosLightSpace;
} fs_in;

uniform uint u_LowerBits;
uniform uint u_UpperBits;

//uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform sampler2D Texture1;

uniform mat4 u_LightSpace;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

void main()
{
	vec2 texel = min(floor((fs_in.TexCoords)*8),7);
	uvec2 bits = uvec2(texel);

	uint X = 7-bits.x;
	uint Y = bits.y;
	uint N = X + 8*Y;
	uint M = (N<32) ? N : N - 32;

	// A, B are 2 32 bit ints representing int C (equivalent 64 bits)

	// outline
	uint A = 0x818181FF;
	uint B = 0xFF818181;

	//uint A = u_LowerBits;
	//uint B = u_UpperBits;

	uint C = (N<32) ? A : B;

	float Result = float(C>>M&1);



	float Border = ( Y==7||X==0||Y==0||X==7) ? 1.0 : 0.0;


    vec4 TexCoordDebug = vec4(texel/8,0.0,1.0);
    

    


    FragColor = fs_in.Color;// = vec4(Result, Result, Result, 1.0);
    //FragColor *= lighting;
}
	