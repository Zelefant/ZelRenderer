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

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	unsigned char* bytes = LoadTextureImage(image, &widthImg, &heightImg, &numColCh);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + slot); // adding to GL_TEXTURE0 allows enumeration using the uint "slot"
	this->unit = slot;
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &id);
}