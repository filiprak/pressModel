#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * transform * vec4(position, 1.0f);
	FragPos = vec3(model * transform * vec4(position, 1.0f));
	Normal = vec3(normalize(transform * vec4(normal, 0.0f)));
	TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);
} 