#version 330 

out vec4 fragColor; 

in vec3 TexCoords; 
uniform samplerCube skyboxTexture; 
in vec3 fragPosition;

uniform float fogMaxDistance = 20; 
uniform float fogMinDistance = 0.1;
uniform vec4 fogColor = vec4 ( 0.5, 0.5, 0.5, 1); 

void main() 
{

	float distanceToFragment = length ( fragPosition ); // distance to fragment in camera space
	float fogImpact = ( fogMaxDistance - distanceToFragment ) / ( fogMaxDistance - fogMinDistance ); 
	fogImpact = clamp ( fogImpact, 0.0, 1.0); 

	vec4 TextureColor = texture ( skyboxTexture, TexCoords );
	fragColor = mix ( fogColor, TextureColor, fogImpact );
}