#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;


uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_LightSpace;
uniform float u_TexIndex;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	float TexIndex;
    vec4 Color;
    vec4 FragPosLightSpace;
} vs_out;



void main()
{

	vs_out.FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(u_Transform))) * a_Normal;
    vs_out.TexCoords = a_TexCoord;
    vs_out.TexIndex = u_TexIndex;
    vs_out.Color = a_Color;
    vs_out.FragPosLightSpace = u_LightSpace * vec4(vs_out.FragPos, 1.0);
    gl_Position = u_ViewProjection * vec4(vs_out.FragPos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	float TexIndex;
    vec4 Color;
    vec4 FragPosLightSpace;
} fs_in;

//uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform mat4 u_LightSpace;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

uniform float u_BezierPoints[16];
uniform int u_PointIndices[8];


vec4 rand = vec4(0.1,0.2,0.3,0.4);
int rand_index = 0;

float ShadowCalculation(vec4 fragPosLightSpace, float inbias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)

    vec2 texelSize = 1.0/textureSize(shadowMap,0);

    int M = 2;
    float weight = 1.0/((M+2)*(M+2));
    float shadow = 0.0;
    for (int y = -M; y <= M; y++)
    {
         for (int x = -M; x <= M; x++)
         {
            
            float closestDepth = texture(shadowMap, projCoords.xy + vec2(x,y)*texelSize).r; 
            float currentDepth = projCoords.z;
            float inshadow = currentDepth - inbias > closestDepth  ? 1.0 : 0.0;  

            shadow += inshadow;
         }
    }


    return shadow*weight;
} 
vec3 LightingCalculation()
{
    vec3 lightPos = u_LightPosition;
    vec3 viewPos = u_CameraPosition;


    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));   
    return lighting;
}

float dot2( in vec2 v ) { return dot(v,v); }
float cro( in vec2 a, in vec2 b ) { return a.x*b.y - a.y*b.x; }

float udSegment( in vec2 p, in vec2 a, in vec2 b )
{
	vec2 pa = p - a;
	vec2 ba = b - a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	return length( pa - ba*h );
}

// signed distance to a quadratic bezier
float sdBezier( in vec2 pos, in vec2 A, in vec2 B, in vec2 C )
{    
    vec2 a = B - A;
    vec2 b = A - 2.0*B + C;
    vec2 c = a * 2.0;
    vec2 d = A - pos;
    float kk = 1.0/dot(b,b);
    float kx = kk * dot(a,b);
    float ky = kk * (2.0*dot(a,a)+dot(d,b)) / 3.0;
    float kz = kk * dot(d,a);      
    float res = 0.0;
    float p = ky - kx*kx;
    float p3 = p*p*p;
    float q = kx*(2.0*kx*kx-3.0*ky) + kz;
    float h = q*q + 4.0*p3;
    if( h >= 0.0) 
    { 
        h = sqrt(h);
        vec2 x = (vec2(h,-h)-q)/2.0;
        vec2 uv = sign(x)*pow(abs(x), vec2(1.0/3.0));
        float t = clamp( uv.x+uv.y-kx, 0.0, 1.0 );
        res = dot2(d + (c + b*t)*t);
    }
    else
    {
        float z = sqrt(-p);
        float v = acos( q/(p*z*2.0) ) / 3.0;
        float m = cos(v);
        float n = sin(v)*1.732050808;
        vec3  t = clamp(vec3(m+m,-n-m,n-m)*z-kx,0.0,1.0);
        res = min( dot2(d+(c+b*t.x)*t.x),
                   dot2(d+(c+b*t.y)*t.y) );
        // the third root cannot be the closest
        // res = min(res,dot2(d+(c+b*t.z)*t.z));
    }
    return sqrt( res );
}


void main()
{
    vec2 p0 = vec2(u_BezierPoints[2*u_PointIndices[0]], u_BezierPoints[2*u_PointIndices[0]+1]);
    vec2 p1 = vec2(u_BezierPoints[2*u_PointIndices[1]], u_BezierPoints[2*u_PointIndices[1]+1]);
    vec2 p2 = vec2(u_BezierPoints[2*u_PointIndices[2]], u_BezierPoints[2*u_PointIndices[2]+1]);
    vec2 p3 = vec2(u_BezierPoints[2*u_PointIndices[3]], u_BezierPoints[2*u_PointIndices[3]+1]);
    vec2 p4 = vec2(u_BezierPoints[2*u_PointIndices[4]], u_BezierPoints[2*u_PointIndices[4]+1]);
    vec2 p5 = vec2(u_BezierPoints[2*u_PointIndices[5]], u_BezierPoints[2*u_PointIndices[5]+1]);
    vec2 p6 = vec2(u_BezierPoints[2*u_PointIndices[6]], u_BezierPoints[2*u_PointIndices[6]+1]);
    vec2 p7 = vec2(u_BezierPoints[2*u_PointIndices[7]], u_BezierPoints[2*u_PointIndices[7]+1]);


	vec2 p = (2.0*fs_in.TexCoords-1.0);

    
    float A = sdBezier( p, p0,          p1,       0.5*(p1+p2) ); 
    float B = sdBezier( p, 0.5*(p1+p2), p2,       0.5*(p3+p4) ); 
    float C = sdBezier( p, p5, p6,       p7 ); 

    float D = min(min(A,B),C);

    float E = 1.0 - float((abs(D)<0.03)); 
    vec3 bezier = vec3(E); 

    FragColor = vec4(LightingCalculation()*bezier*fs_in.Color.rgb,1.0);
}