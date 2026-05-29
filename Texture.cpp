#include "Texture.h"

unsigned char* LoadTextureImage(const char* path, int* widthImg, int* heightImg, int* numColCh)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(path, widthImg, heightImg, numColCh, 0);
	if (!bytes)
	{
		std::cout << "Error loading texture " << path << std::endl;
		throw errno;
	}

	return bytes;
}

Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = LoadTextureImage(image, &widthImg, &heightImg, &numColCh);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + slot); // adding to GL_TEXTURE0 allows enumeration using the uint "slot"
	this->unit = slot;
	glBindTexture(texType, id);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);

	this->type = texType;

}

// Modify a texture uniform
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.id, uniform);
	shader.Activate();
	glUniform1i(texUni, 0);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, id);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &id);
}