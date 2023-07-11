#version 330 core
out vec4 fragColor; 
in vec2 texCoords; 
uniform sampler2D texSampler; 

uniform ivec2 pattern = ivec2 (4,4); 
uniform int frame;  

void main () 
{
	float x = frame % pattern.x; 
	float y = frame / pattern.y; 
	
	x += texCoords.x; 
	y += texCoords.y; 


	vec2 coords = vec2( x / pattern.x, y / pattern.y ); 

	fragColor = texture ( texSampler, coords ); 
	if ( fragColor.a < 0.1 ) // idk why but GL_BLEND ( SRC_ALPHA, SRC_ONE_MINUS_SRC_ALPHA ) doesnt work at all
	    discard;
		
	// fragColor = texture ( texSampler, texCoords ); 
}