#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambient = 0.1f;
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDir), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDir, normal);

	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 8);
	float specular = specAmount * specularLight;

	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}