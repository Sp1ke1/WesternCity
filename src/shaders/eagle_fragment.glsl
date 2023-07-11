#version 330 core
out vec4 fragColor;


in vec2 texCoords; 
in vec3 fragPosition;

uniform sampler2D texture_diffuse1;


uniform float fogMaxDistance = 20; 
uniform float fogMinDistance = 0.1;
uniform vec4 fogColor = vec4 ( 0.5, 0.5, 0.5, 1); 


void main()
{
	
	

	// Apply fog
	float distanceToFragment = length ( fragPosition ); // distance to fragment in camera space
	float fogImpact = ( fogMaxDistance - distanceToFragment ) / ( fogMaxDistance - fogMinDistance ); 
	fogImpact = clamp ( fogImpact, 0.0, 1.0); 
	vec4 TextureColor = texture ( texture_diffuse1, texCoords ); 

	fragColor = mix( fogColor, TextureColor, fogImpact );
	// fragColor = vec4 (ResultColor, 1);
}