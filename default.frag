#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 Normal;
in vec3 crntPos;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 PointLight()
{
	// Intensity formula -- Modification of inverse square formula.
	// Inverse of a quadratic equation where a controls how fast the light dies out and b controls how far it reaches.
	// a and b can be >1 but are typically between 0 and 1 for most purposes
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);


	// Ambient Lighting
	float ambient = 0.1f;

	// Diffuse Lighting
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightVec);

	float diffuse = max(dot(normal, lightDir), 0.0f);

	// Specular Lighting
	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDir, normal);

	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;

	// Added altogether
	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 DirectLight()
{
	// Ambient Lighting
	float ambient = 0.1f;

	// Diffuse Lighting
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 0.0f));

	float diffuse = max(dot(normal, lightDir), 0.0f);

	// Specular Lighting
	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDir, normal);

	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;

	// Added altogether
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 SpotLight()
{
	// Controls cone shape of spot light.
	float outerCone = 0.70f;
	float innerCone = 0.90f;

	// Ambient Lighting
	float ambient = 0.1f;

	// Diffuse Lighting
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDir), 0.0f);

	// Specular Lighting
	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDir, normal);

	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	// Added altogether
	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}


void main()
{
	FragColor = SpotLight();
}