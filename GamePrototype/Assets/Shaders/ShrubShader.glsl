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
#include "../EDNA/src/EDNA/Shaders/Noise.glsl"

layout (triangles, invocations = 2) in;
layout (triangle_strip, max_vertices = 32) out;


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
uniform float u_Time;
uniform float u_Angle;

const float MAGNITUDE = 0.4;

uniform mat4 u_ViewProjection;

mat2 rotate2D(float theta) {
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    
    return mat2(cosTheta, -sinTheta, sinTheta, cosTheta);
}

void EmitBaseMesh()
{
    for (int i = 0; i < gl_in.length; i ++)
    {
        gl_Position = u_ViewProjection * gl_in[i].gl_Position;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.Color = gs_in[i].Color;
        gs_out.TexCoords = gs_in[i].TexCoords;
        EmitVertex();
    }
     EndPrimitive();
}

void EmitLeaf(vec3 pos, float angle_z, float size)
{

    float zr = angle_z*2*3.141592653589;



    // Left side
    gl_Position = u_ViewProjection * ( vec4(pos,1) + size*vec4( rotate2D(zr) * vec2(0.5, 0.5), 0.4,0)  );
    gs_out.Normal = vec3(0,-1,0);
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(0,1);
    EmitVertex();


    // Stem
    gl_Position = u_ViewProjection * ( vec4(pos,1) + size*vec4( rotate2D(zr) * vec2(0,  0),   0.1,0)  );
    gs_out.Normal = vec3(0,-1,0);
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(0,0);
    EmitVertex();


    // Tip
    gl_Position = u_ViewProjection * ( vec4(pos,1) + size*vec4( rotate2D(zr) * vec2(1,  0),     0.2,0)  );
    gs_out.Normal = vec3(0,-1,0);
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(1,1);
    EmitVertex();

    // Left side
    gl_Position = u_ViewProjection * ( vec4(pos,1)  + size*vec4( rotate2D(zr) * vec2(0.5,-0.5),    0.4,0)  );
    gs_out.Normal = vec3(0,-1,0);
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(1,0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    //EmitBaseMesh();
    vec3 AveragePos = (gs_in[0].FragPos + gs_in[1].FragPos + gs_in[2].FragPos)/3.0;
    vec3 AverageNormal = (gs_in[0].Normal + gs_in[1].Normal + gs_in[2].Normal)/3.0;
    vec4 AverageColor = (gs_in[0].Color + gs_in[1].Color + gs_in[2].Color)/3.0;

    int LEAVES = 3;
    vec3 NewPos = vec3(AveragePos.x ,AveragePos.y + (AverageColor.w + 1)/2.0, AveragePos.z);
    float Size = 0.1*dot(AverageColor,AverageColor);
    Size *= float(Size>0.1);


    for (int l = 0; l < LEAVES; l++)
    {
        float r = float(l)/float(LEAVES);

        EmitLeaf(NewPos,  r  + AverageColor.x + AverageColor.y, Size);
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


float line_segment(in vec2 p, in vec2 a, in vec2 b) {
	vec2 ba = b - a;
	vec2 pa = p - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0., 1.);
	return length(pa - h * ba);
}


void main()
{
    vec2 uv = fs_in.TexCoords;

    float X = 2*uv.x-1;
    float Y = 2*uv.y-1;

    float stem_width = 0.15;
    float stem_angle = 0.067;
    vec4 stem_color = vec4(0.5,0.7,0.4,1);
    vec4 leaf_color = vec4(0.1,0.4,0.1,1);

    float Z = max(0,stem_width - abs(uv.x - uv.y) - stem_angle*abs(uv.x+uv.y))/stem_width;

    bool out_of_shape = (X*X > 1-Y*Y) && (X*Y < 0);
    if (out_of_shape){discard;}

    float L = line_segment(uv,vec2(0,0),vec2(1,1));
    L = 1-L;
    L*=L;
    L = fract(5*X)+fract(5*Y)-1;
    L = (L+1)/2.0;


    Z *= Z;

    FragColor = vec4(uv.x,uv.y,0,1);// fs_in.Color;// = vec4(Result, Result, Result, 1.0);
    FragColor = (1-Z)*leaf_color + (Z)*stem_color;
    //FragColor *= lighting;
}
	