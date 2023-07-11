#version 330 core
out vec4 fragColor;


in vec2 texCoords; 
in vec3 fragPosition;
in vec3 normal; 


uniform vec3 cameraPosition; 
uniform mat4 VMatrix; 

// Lights 
struct PointLight
{
	vec3 position; 

	vec3 ambientColor; 
	vec3 diffuseColor; 
	vec3 specularColor; 

	float constant; 
	float linear; 
	float quadratic; 
};

struct SpotLight 
{
	vec3 position; 
	vec3 direction; 
	
	vec3 ambientColor;
	vec3 diffuseColor; 
	vec3 specularColor; 

	float cutoff;
	float exponent; 

	float constant; 
	float linear;
	float quadratic; 
};

struct DirectionalLight
{
	vec3 direction; 
	
	vec3 ambientColor; 
	vec3 diffuseColor; 
	vec3 specularColor; 
};


// material 

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	bool hasSpecular;
	
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};
uniform Material material; 
uniform float fogMaxDistance = 20; 
uniform float fogMinDistance = 0.1;
uniform vec4 fogColor = vec4 ( 0.5, 0.5, 0.5, 1); 




// Point lights
#define MAX_POINT_LIGHTS 16
uniform int nOfPointLights; 
uniform PointLight pointLights [MAX_POINT_LIGHTS]; 
uniform bool isPointLightActive[MAX_POINT_LIGHTS]; // TODO: move to light struct

// Directional lights
uniform DirectionalLight directionalLight; 
uniform bool isSpotlightActive; 
// Spotlights. TODO: make variable number of spot lights
uniform SpotLight spotLight; 



vec3 CalculateDirectionalLightImpact ( DirectionalLight light, vec3 normal, vec3 viewDir, vec3 textureDiffuse, vec3 textureSpecular )
{
	vec3 lightDirCameraSpace = normalize((VMatrix * vec4(light.direction, 0)).xyz);
	vec3 L = normalize(lightDirCameraSpace); // light direction 
    vec3 R = reflect(-L, normal); // reflected light direction
	vec3 V = viewDir; // direction to viewer

	float diffuseImpact = max(dot(L, normal), 0.0); 
	float specularImpact = pow(max(dot(V, R), 0.0), material.shininess);

    vec3 ambient  = light.ambientColor * material.ambientColor * textureDiffuse; 
    vec3 diffuse  = light.diffuseColor  * diffuseImpact * material.diffuseColor * textureDiffuse; 
	vec3 specular; 
	if ( material.hasSpecular ) 
		specular = light.specularColor * specularImpact * textureSpecular;
	else
		specular = light.specularColor * specularImpact * material.specularColor;
    return vec3(ambient + diffuse + specular);
};


vec3 CalculatePointLightImpact ( PointLight light, vec3 normal, vec3 viewDir, vec3 textureDiffuse, vec3 textureSpecular ) 
{
	vec3 lightPositionCameraSpace = (VMatrix * vec4(light.position,1)).xyz;
	vec3 L = normalize(lightPositionCameraSpace - fragPosition); // light direction 
    vec3 R = reflect(-L, normal); // reflected light direction
	vec3 V = viewDir; // direction to viewer

	float diffuseImpact = max(dot(L, normal), 0.0); 
	float specularImpact = pow(max(dot(V, R), 0.0), material.shininess);

    vec3 ambient  = light.ambientColor * material.ambientColor * textureDiffuse; 
    vec3 diffuse  = light.diffuseColor  * diffuseImpact * material.diffuseColor * textureDiffuse; 
    vec3 specular; 
	if ( material.hasSpecular ) 
		specular = light.specularColor * specularImpact * textureSpecular;
	else
		specular = light.specularColor * specularImpact * material.specularColor;

	float distanceToLight = distance ( lightPositionCameraSpace, fragPosition );  
    float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + 
  			     light.quadratic * (distanceToLight * distanceToLight));

	vec3 lightImpact = ambient + diffuse + specular; 
	
	return lightImpact * attenuation; 
	
};

vec3 CalculateSpotlightImpact ( SpotLight light, vec3 normal, vec3 viewDir, vec3 textureDiffuse, vec3 textureSpecular ) 
{
	vec3 lightPositionCameraSpace = vec3 ( 0, 0, 0 ); 
	// vec3 lightPositionCameraSpace = normalize(VMatrix * vec4(light.position,1)).xyz;
	// vec3 lightDirCameraSpace = normalize((VMatrix * vec4(light.direction, 0.0)).xyz);
	vec3 lightDirCameraSpace = vec3 ( 0, 0, -1 ); 
	vec3 L = normalize(lightPositionCameraSpace - fragPosition ); // direction from fragment to light
	float theta = dot ( lightDirCameraSpace, -L ); 

	if ( theta < light.cutoff )
	{
		return vec3 (0,0,0); 
	}

	float intensity = pow(theta, light.exponent); 

    vec3 R = reflect(-L, normal); // reflected light direction
	vec3 V = viewDir; // direction to viewer


	float diffuseImpact = max(dot(L, normal), 0.0); 
	float specularImpact = pow(max(dot(V, R), 0.0), material.shininess);

    vec3 ambient  = light.ambientColor * material.ambientColor * textureDiffuse; 
    vec3 diffuse  = light.diffuseColor  * diffuseImpact * material.diffuseColor * textureDiffuse; 

	vec3 specular; 
	if ( material.hasSpecular ) 
		specular = light.specularColor * specularImpact * textureSpecular;
	else
		specular = light.specularColor * specularImpact * material.specularColor;

    float distanceToLight = distance ( vec3(0), fragPosition );  
    float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * (distanceToLight * distanceToLight)); 

	vec3 lightImpact = ambient + diffuse + specular; 
	
	return lightImpact * intensity * attenuation; 
}

void main()
{
	
	vec3 ResultColor = vec3(0); 
	vec3 nNormal = normalize ( normal ); 
	vec3 viewDir = normalize ( -fragPosition ); 
	
	vec3 textureDiffuse = vec3(texture(material.texture_diffuse1, texCoords)); 
	vec3 textureSpecular = vec3 ( 1 );
	if ( material.hasSpecular ) 
		textureSpecular = vec3(texture(material.texture_specular1, texCoords));


	// Apply lights 
	ResultColor += CalculateDirectionalLightImpact ( directionalLight, nNormal, viewDir, textureDiffuse, textureSpecular ); 
	for ( int i = 0; i < nOfPointLights; i ++ ) 
	{
		if ( isPointLightActive [ i ] )
			ResultColor += CalculatePointLightImpact ( pointLights[i], nNormal, viewDir, textureDiffuse, textureSpecular ); 
	}
	if ( isSpotlightActive ) 
	{
		ResultColor += CalculateSpotlightImpact ( spotLight, nNormal, viewDir, textureDiffuse, textureSpecular ); 
	}

	// Apply fog
	float distanceToFragment = length ( fragPosition ); // distance to fragment in camera space
	float fogImpact = ( fogMaxDistance - distanceToFragment ) / ( fogMaxDistance - fogMinDistance ); 
	fogImpact = clamp ( fogImpact, 0.0, 1.0); 

	fragColor = mix( fogColor, vec4(ResultColor, 1), fogImpact );
	// fragColor = vec4 (ResultColor, 1);
}