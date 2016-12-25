#version 330 core
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 color;

// testures
uniform sampler2D weitiTexture;
uniform sampler2D iipwTexture;
uniform sampler2D komanTexture;
uniform sampler2D metalTexture;
uniform sampler2D shadowMap;

// passed data
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float ambientIntensity;
uniform vec3 meshColor;
uniform vec3 camPosition;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	// PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - 0.1 * bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
	vec3 ambientLight = ambientIntensity * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseLight = lightIntensity * diff * lightColor;

	// reflect light specification
	float specIntensity = 0.8f;
	vec3 viewDir = normalize(camPosition - FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specIntensity * spec * lightColor; 

	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005); 
	float shadow = ShadowCalculation(FragPosLightSpace, bias);
	vec3 result = (ambientLight + (1.0 - shadow) * (diffuseLight + specular)) * meshColor;

    vec3 tex = (ambientLight + (1.0 - shadow) * (diffuseLight + specular)) * texture(metalTexture, TexCoords);

	color = vec4(result, 1.0f);
}
