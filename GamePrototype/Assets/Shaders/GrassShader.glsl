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

layout (triangles, invocations = 1) in;
layout (triangle_strip, max_vertices = 75 ) out;


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
        gs_out.Color = vec4(0,0.25,0.1,1.0);//gs_in[i].Color;//+  
        gs_out.TexCoords = gs_in[i].TexCoords;
        gs_out.Normal = gs_in[i].Normal;
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

    float smooth_color = (sin(pos.x) + sin(pos.y))/2.f; 
    float dc = fract(r)/8.f;

    float dx = 0.01*sin(2.0*3.141592653*u_Time/10.f + r/10.0);
    float dy = 0.01*cos(2.0*3.141592653*u_Time/11.f + t/10.0);

    vec2 blade_uv_offset = vec2(1,0);

    //bottom left
    gs_out.TexCoords = vec2(0,0) + blade_uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (pos + vec4(t*ds-1.5*dw,q*ds,0,0));
    EmitVertex();

    //top middle point
    gs_out.TexCoords = vec2(0.5,1) + blade_uv_offset;
    gs_out.Color = vec4(0.1+3*dc,0.5+dc,0,1); 
    gl_Position = u_ViewProjection * (pos + 1.5*vec4(t*ds +dw*(t+q)  + dx , q*ds+2*dw + dy , (0.5 + 0.2*(t*q))  ,0  ) );
    EmitVertex();

    //bottom right point
    gs_out.TexCoords = vec2(1,0) + blade_uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (pos + vec4(t*ds+1.5*dw,q*ds,0,0));
    EmitVertex();
    EndPrimitive();
}

void main()
{
    Primitive p = PassBaseMesh();

    vec4 c = p.AverageColor;

    for (int i = 0; i < 3; i++)
    {
    AddGrass(p.AveragePos,p.AverageNormal,c);
    c = fract((i/3.0 - 1.414*c)*c);
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
    vec2 uv = vec2(fract(fs_in.TexCoords.x),fs_in.TexCoords.y);

    float width = 1.0/2.0;
    float blend = 2.0/width * max(width/2.0 - abs(uv.x - 1.0/2.0),0);
    bool BLADE = fs_in.TexCoords.x > 1;

    if ((blend == 0 || uv.y > 0.90f) && BLADE) {discard;}


    float a = blend;
    float b = 3.0f;
    float Y = (1+b)*min(uv.y/b,1-uv.y);  

    a*= a;
    a*= a;
    a*= (1-Y);


    
   
    vec4 blade_color = vec4((1-a)*fs_in.Color.x  +  (a)*0.1  ,
                            (1-a)*fs_in.Color.y  +  (a)*0.25 ,
                            (1-a)*fs_in.Color.z  +  (a)*0.1  ,
                             1.0);


//blade_color = vec4(Y,0,0,1);

    FragColor = float(BLADE)*blade_color + (1.0-float(BLADE))*fs_in.Color;// = vec4(Result, Result, Result, 1.0);
    //FragColor *= lighting;
}
	