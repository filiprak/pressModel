#version 330 core
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 color;

// testures
uniform sampler2D weitiTexture;
uniform sampler2D iipwTexture;
uniform sampler2D komanTexture;
uniform sampler2D metalTexture;

// passed data
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float ambientIntensity;
uniform vec3 meshColor;
uniform vec3 camPosition;

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

	vec3 result = (diffuseLight + ambientLight + specular) * meshColor;
    vec3 tex = (diffuseLight + ambientLight + specular) * texture(metalTexture, TexCoords);

	color = vec4(result, 1.0f);
}
