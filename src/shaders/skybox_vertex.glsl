#version 330 core
 

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal; 
// layout (location = 2) in vec2 aTexCoords; 

uniform mat4 PVM;
uniform mat4 M; 
uniform mat4 V; 
out vec3 TexCoords;
out vec3 fragPosition; 


void main() 
{
	TexCoords = aPos; 
	fragPosition = (V * M * vec4(aPos,1)).xyz; 
	gl_Position = PVM * vec4 ( aPos, 1.0); 
	
}