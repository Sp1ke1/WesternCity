#version 330 core
 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aTexCoords; 

uniform mat4 PVMMatrix; 
uniform mat4 PMatrix; 
uniform mat4 VMatrix; 
uniform mat4 MMatrix; 
uniform bool IsWater; 
uniform mat4 WaterTransform; 

out vec2 texCoords; 
out vec3 normal; 
out vec3 fragPosition; 

void main()
{
    normal = mat3(VMatrix) * mat3(transpose(inverse(MMatrix))) * aNormal; 
    if ( !IsWater )
    {
        texCoords = aTexCoords;
    }
    else
    {
        texCoords = (WaterTransform * vec4 (aTexCoords, 1, 1) ).xy; 
    }
    fragPosition = vec3( VMatrix * MMatrix * vec4(aPos, 1.0f)).xyz;
    gl_Position = PVMMatrix * vec4 ( aPos, 1 );
}