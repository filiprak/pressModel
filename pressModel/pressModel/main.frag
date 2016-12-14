#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


void main()
{
	float ambientStrength = 0.2f;
	vec3 ambientLight = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseLight = diff * lightColor;

	float specularStrength = 0.9f;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 result = (diffuseLight + ambientLight + specular) * objectColor;
    color = vec4(result, 1.0f);
}
