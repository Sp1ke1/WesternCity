#version 330 core
 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNextPos;
layout (location = 2) in vec2 aTexCoords;


uniform mat4 PVMMatrix; 
uniform mat4 PMatrix; 
uniform mat4 VMatrix; 
uniform mat4 MMatrix; 
 
uniform float AnimationAlpha; 

out vec2 texCoords; 
out vec3 normal; 
out vec3 fragPosition; 

void main()
{

    vec3 position = mix ( aPos, aNextPos, AnimationAlpha );
    texCoords = aTexCoords; 
    fragPosition = vec3( VMatrix * MMatrix * vec4(position, 1)).xyz;
    gl_Position = PVMMatrix * vec4 ( position, 1 );
}