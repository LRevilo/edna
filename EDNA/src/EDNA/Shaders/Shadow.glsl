#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_LightSpace;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_LightSpace * u_Transform * vec4(aPos, 1.0);
}  


#type fragment
#version 330 core


void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}  