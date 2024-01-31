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
uniform vec3 u_PlayerPosition;
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
    vec4 AverageColor = (gs_in[0].Color + gs_in[1].Color + gs_in[2].Color)/3.0;
    for (int i = 0; i < gl_in.length; i ++)
    {
        gl_Position = u_ViewProjection * gl_in[i].gl_Position;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.Color = AverageColor;
        gs_out.TexCoords = gs_in[i].TexCoords;
        EmitVertex();
    }
     EndPrimitive();
}
void EmitGrass(vec3 pos, float size)
{
    vec2 uv_offset = vec2(0,1);

    //bottom left
    gs_out.TexCoords = vec2(0,0) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * ( vec4(pos,1) + size * vec4(-0.125,0,0,0) );
    EmitVertex();

    //top middle point
    gs_out.TexCoords = vec2(0.5,1) +uv_offset;
    gs_out.Color = vec4(0.1,0.5,0,1); 
    gl_Position = u_ViewProjection * ( vec4(pos,1) + size * vec4(0,0,1,0) );
    EmitVertex();

    //bottom right point
    gs_out.TexCoords = vec2(1,0) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * ( vec4(pos,1) + size * vec4(0.125,0,0,0) );
    EmitVertex();
    EndPrimitive();
}
void EmitStem(vec3 pos, float size)
{
    vec2 uv_offset = vec2(0,2);

        //Bottom Left
    gs_out.TexCoords = vec2(0,0) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(-0.025,0,0,0));
    EmitVertex();

        // Top Left
    gs_out.TexCoords = vec2(0,1) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(-0.025,0,1,0));
    EmitVertex();

    // Bottom Right
    gs_out.TexCoords = vec2(1,0) + uv_offset;
    gs_out.Color = vec4(0.1,0.5,0,1); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0.025, 0, 0,0 ));
    EmitVertex();

        // Top Right
    gs_out.TexCoords = vec2(1,1) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0.025,0,1,0));
    EmitVertex();


    EndPrimitive();
}
void EmitLeaf(vec3 pos, float angle_z, float size)
{
    vec2 uv_offset = vec2(0,3);
    float zr = angle_z*2*3.141592653589;

    //   3      X
    //         /|\
    //        / | \
    //       /  |  \
    //   1  X   |   X  4
    //       \  |  /
    //        \ | /
    //         \|/
    //   2      X

    float r = fract(420.696969*zr);
    r = fract(420*3.141592653589*r*r);
    r = (2.0*r- 1.0)/8.0;
    size *= (1 - r);
   
    vec3 p1 = size*vec3( rotate2D(zr)*vec2(0.5,  0.5), 0.4)  ;
    vec3 p2 = size*vec3( rotate2D(zr)*vec2(0,    0  ), 0.1)  ;
    vec3 p3 = size*vec3( rotate2D(zr)*vec2(1,    0  ), 0.2)  ;
    vec3 p4 = size*vec3( rotate2D(zr)*vec2(0.5, -0.5), 0.4)  ;

    vec3 n1 = cross(p1-p2,p1-p3);
    vec3 n4 = -cross(p4-p2,p4-p3);

    // 1. Left side
    gl_Position = u_ViewProjection * vec4(p1 + pos,1);
    gs_out.Normal = n1;
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(0,1) + uv_offset;
    EmitVertex();


    // 2. Stem
    gl_Position = u_ViewProjection * vec4(p2 + pos,1);
    gs_out.Normal = n1;
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(0,0) + uv_offset;
    EmitVertex();

    // 3. Tip
    gl_Position = u_ViewProjection * vec4(p3 + pos,1);
    gs_out.Normal = n1;
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(1,1) + uv_offset;
    EmitVertex();

    EndPrimitive();



    // 4. Right side
    gl_Position = u_ViewProjection * vec4(p4 + pos,1);
    gs_out.Normal = n4;
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(1,0) + uv_offset;
    EmitVertex();

    // 3. Tip
    gl_Position = u_ViewProjection * vec4(p3 + pos,1);
    gs_out.Normal = n4;
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(1,1) + uv_offset;
    EmitVertex();

    // 2. Stem
    gl_Position = u_ViewProjection * vec4(p2 + pos,1);
    gs_out.Normal = n4;
    gs_out.Color = vec4(0.5,0,0,1);
    gs_out.TexCoords = vec2(0,0) + uv_offset;
    EmitVertex();

    EndPrimitive();
    
}

void EmitPetals(vec3 pos, float size, float curvature)
{

    float H = clamp(curvature,-1,1);
    vec2 uv_offset = vec2(0,4);

    //part 1

    // Bottom left
    gs_out.TexCoords = vec2(0,0) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(-0.5,-0.5,H,0));
    EmitVertex();

    // middle
    gs_out.TexCoords = vec2(0.5,0.5) + uv_offset;
    gs_out.Color = vec4(0.1,0.5,0,1); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0, 0, 0,0 ));
    EmitVertex();

    // bottom right
    gs_out.TexCoords = vec2(1,0) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0.5,-0.5,H,0));
    EmitVertex();

    //Top right
    gs_out.TexCoords = vec2(1,1) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0.5,0.5,H,0));
    EmitVertex();
    EndPrimitive();

    // part 2

    // Bottom left
    gs_out.TexCoords = vec2(0,0) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(-0.5,-0.5,H,0));

    EmitVertex();

    // middle
    gs_out.TexCoords = vec2(0.5,0.5) + uv_offset;
    gs_out.Color = vec4(0.1,0.5,0,1); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0, 0, 0, 0 ));
    EmitVertex();

    // Top left
    gs_out.TexCoords = vec2(0,1) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(-0.5,0.5,H,0));
    EmitVertex();

    //Top right
    gs_out.TexCoords = vec2(1,1) + uv_offset;
    gs_out.Color = vec4(0,0.25,0.1,1.0); 
    gl_Position = u_ViewProjection * (vec4(pos,1) + size*vec4(0.5,0.5,H,0));
    EmitVertex();
    EndPrimitive();



}
void EmitClover(vec3 pos, float size, float rotation)
{
    const int LEAVES = 2;
    const float INVLEAVES = 1.0/LEAVES;
    EmitStem(pos,size);
    vec3 tip = vec3(pos.x,pos.y,pos.z+  size);
    for (int i = 0; i < LEAVES; i++)
    {
        EmitLeaf(tip,  i*INVLEAVES + rotation, size*0.45);
    }
}

void EmitFlower(vec3 pos, float size)
{
    float s = 0.5*size;
    EmitStem(pos,s);
    vec3 tip = vec3(pos.x,pos.y,pos.z + s);
    EmitPetals(tip, s, 0.5);
}



void main()
{
    EmitBaseMesh();
    vec3 AveragePos = (gs_in[0].FragPos + gs_in[1].FragPos)/2.0;
    vec3 AverageNormal = (gs_in[0].Normal + gs_in[1].Normal + gs_in[2].Normal)/3.0;
    vec4 AverageColor = (gs_in[0].Color + gs_in[1].Color + gs_in[2].Color)/3.0;

    

    float x_offset = 0.5*(gs_in[0].Color.x + gs_in[2].Color.x - 1);
    float y_offset = 0.5*(gs_in[2].Color.x + gs_in[1].Color.x - 1);


    vec3 NewPos = vec3(AveragePos.x + x_offset ,AveragePos.y + y_offset, AveragePos.z);
    float Size = 0.5*AverageColor.x;
    //0.5*dot(AverageColor,AverageColor);
    //Size *= 3*Size*(int(1331771*Size)&1); //float(Size>0.3)*float(Size<0.6)*
    //gs_out.Color = AverageColor;
    
    if (AverageColor.x > 0.5 && fract(1313131.0*AverageColor.x) > 0.5) {EmitClover(NewPos, Size, AverageColor.x-2*AverageColor.y);}
    else if (AverageColor.x < 0.5 && fract(1313131.0*AverageColor.x) > 0.5) {EmitFlower(NewPos, 2*Size);}
    else 
    { 
        int BLADES = 6;
        for (int i = 0; i < BLADES; i++)
        {
            float rx = 2*fract(9841.2121*AverageColor.x - x_offset*(i+1)*(i+2)*AverageColor.z) - 1;
            float ry = 2*fract(3242.1543*AverageColor.y  + y_offset*(BLADES*BLADES - i*i)*AverageColor.w) - 1;
            float rs = 2*fract(fract(rx+ry) - fract(rx*ry))-1; 

            vec3 pos = vec3(NewPos.x + rx/2.0, NewPos.y + ry/2.0, NewPos.z);
            EmitGrass(pos ,Size*(1+rs/8.0));

        }
    }
    //EmitStem(NewPos,Size);
    //EmitPetals(NewPos,Size);



} 
///////////////////////////////////////////////////////////////////////////////////////////////////
#type fragment
#version 410 core
#include "../EDNA/src/EDNA/Shaders/PhongLighting.glsl"
#include "../EDNA/src/EDNA/Shaders/Noise.glsl"


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

float WavTriangleP(float x)
{
    return 2*abs(   (x-0.25) - floor(x-0.25) - 0.5  );
}
float WavTriangleEven(float x)
{
    return 1.0 - 4.0*abs(fract(x-0.5) - 0.5 );
}

void main()
{
    vec2 uv = fs_in.TexCoords;
    
    float x = fract(uv.x);
    float y = fract(uv.y);
    float X = 2*x-1;
    float Y = 2*y-1;

    int idY = int(floor(uv.y));

    bool shapes[5];
    shapes[0] = true;
    shapes[1] = abs(X)-0.5 < 0;// grass
    shapes[2] = true;// stem

    float petals = 5;
    shapes[3] = min(-X*Y, X*X + Y*Y - 1.0) < 0;// leaf
    shapes[4] =  X*X + Y*Y < WavTriangleEven(petals/(2.0*3.141592653589)* Arctan2Type(Y,X));//Y;//X*X + Y*Y - 1.0 < 0;// flower
    if (!shapes[idY]){discard;}


    vec4 colors[5];
    colors[0] = fs_in.Color;
    colors[1] = vec4(0,0.3,0,1);  // grass
    colors[2] = vec4(0.4,0.6,0.4,1);  // stem
    colors[3] = vec4(0.1,0.4,0,1);  // leaf
    colors[4] = vec4(0.5,0.0,0,1);// flower
    vec4 color = colors[idY];
    //





    //out_of_shape = (x*x+y*y)>1.0/2.0;
    //out_of_shape = false;

    // flower shape
    float w = 5.2;
    float a = 0.33;
 
    bool in_flower_1 = (x*x + y*y) < 0.5*(1-w + w*CosType(ArctanType((x-y)/(x+y))));
    bool in_flower_2 = (x*x + y*y) < 0.5*(1-w + w*CosType(a*CosType(a*(x+y)/(x-y))));
    bool in_petal = (x+y) < CosType(a*(x-y)/(x*y));

    //if (!in_petal){discard;}

    

    float stem_width = 0.15;
    float stem_angle = 0.067;
    vec4 stem_color = vec4(0.5,0.7,0.4,1);
    vec4 leaf_color =  vec4(0.1,0.4,0.1,1);
    vec4 detail_color =vec4(0.25,0.45,0.15,1);

    float Z = (1.0/stem_width)*max(0,stem_width - abs(uv.x - uv.y) - stem_angle*abs(uv.x+uv.y));
    Z *= Z;

  

    float L = line_segment(uv,vec2(0,0),vec2(1,1));
    L = 1-L;
    L*=L;
    L = fract(5*X)+fract(5*Y)-1;
    L = (L+1)/2.0;


    //interior leaf extra detail
    // Smoothing function so the path around the square (0,0) (0,1) (1,1)  (1,0) = 0 at the edges
    float S = 16.0*uv.x*uv.y*(1-uv.x)*(1-uv.y);
    float D = 0.1 - 1.45*abs(sin(2*8*uv.x*uv.y)*(uv.x-uv.y))*S;
    //if (D>0){discard;}
    D = float(D<0);
    

    float Fuv = (x + y);

    color.w = 1;


    
    vec4 lighting = Phong_CalculateLighting(u_LightPosition, vec3(1,1,1), u_CameraPosition - u_LightPosition, fs_in.FragPos, fs_in.Normal, 0.0);



    FragColor = color*lighting;

    //FragColor = vec4(1,0.8,0.8,1)*(Fuv) + (1-Fuv)*vec4(0.6,0.2,0.4,1);
    //FragColor = vec4(x,y,0,1);// fs_in.Color;// = vec4(Result, Result, Result, 1.0);


    //FragColor = (1-Z)*leaf_color + (Z)*stem_color;
    //FragColor *= lighting;

    //FragColor = vec4(gl_FrontFacing);


    






}
	