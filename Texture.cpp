#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

Texture::Texture(std::string path, const char* varName, bool flip, 
	int wrapping, int filtering, bool gammaCorrect)
{
	// generate name without extension
	// path expected to be absolute, including path separators
	this->path = path;
	std::string basename = path.substr(path.find_last_of("\\/") + 1);
	std::string delimiter = ".";
	std::string token = basename.substr(0, basename.find(delimiter));

	if (varName == NULL)
		name = token;
	else
		name = static_cast<std::string>(varName);
	
	isFlipped = flip;

	// bind texture
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// if no texture is specified, bail out now
	if (path.empty()) return;
	std::cout << "Loading texture " << name << " ID=" << ID << std::endl;

	// set wrapping / filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "Failed to load texture at " << path.c_str() << std::endl;
		stbi_image_free(data);
		return;
	}

	int format = getFormat();
	int intFormat = getInternalFormat(gammaCorrect);

	// generate the texture + mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat,
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


int Texture::getFormat()
{
	int format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	return format;
}


int Texture::getInternalFormat(bool gammCorrection)
{
	int format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = gammCorrection ? GL_SRGB : GL_RGB;
	else if (nrChannels == 4)
		format = gammCorrection ? GL_SRGB_ALPHA : GL_RGBA;

	return format;
}


Texture::~Texture()
{
	//std::cout << "Deconstructing Texture ID=" << ID << std::endl;
}


CubeMap::CubeMap(std::vector<std::string> faces, std::string name)
{
	this->name = name;

	// bind texture
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	std::vector<GLenum> faceOrder = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,

		// load top and bottom opposite way around
		// cubemaps use left handed coordinates, opengl does not.
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,

		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			int format = getFormat();
			int intFormat = getFormat();
			glTexImage2D(faceOrder[i], 
				0, intFormat, width, height, 0,
				format, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		} 
		else
		{
			std::cout << "Failed to load cubmap at " << path.c_str() << std::endl;
			stbi_image_free(data);
			return;
		}
	}

	// set wrapping / filtering options
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMap::~CubeMap()
{

}