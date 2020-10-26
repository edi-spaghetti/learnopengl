#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

Texture::Texture(const char* path, const char* varName, bool flip, 
	bool hasAlpha, int wrapping, int filtering)
{
	// generate name without extension
	// assumes no subfolders and filename is usable as variable name
	std::string strPath = static_cast<std::string>(path);
	std::string delimiter = ".";
	std::string token = strPath.substr(0, strPath.find(delimiter));

	if (varName == NULL)
	{
		name = token;
	}
	else
	{
		name = static_cast<std::string>(varName);
	}
	

	isFlipped = flip;

	// if no texture is specified, bail out now
	if (!path) return;

	// bind texture
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set wrapping / filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture at " << path << std::endl;
		stbi_image_free(data);
		return;
	}

	int format;
	if (hasAlpha)
	{
		format = GL_RGBA;
	}
	else
	{
		format = GL_RGB;
	}

	// generate the texture + mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		width, height,
		0, format, GL_UNSIGNED_BYTE,
		data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	if (doLogging) 
	{
		std::cout << "Loaded texture " << name << " ID=" << ID << std::endl;
		doLogging = false;
	}

	loaded = true;
}

Texture::~Texture()
{
	std::cout << "Deconstructing Texture ID=" << ID << std::endl;
}
