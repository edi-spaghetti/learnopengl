#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"

// construction and deconstruction
// ---------------------------------------------------------------------------
Shader::Shader(const char* vertPath, const char* fragPath, Geometry geo, const char* texPath)
{
	ID = glCreateProgram();
	int vert_id = Shader::createVertexShader(vertPath);
	int frag_id = Shader::createFragmentShader(fragPath);

	glAttachShader(ID, vert_id);
	glAttachShader(ID, frag_id);
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// tidy up by deleting vert and prog shaders
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	// load geometry
	Shader::loadGeometry(geo);
	geometryLoaded = true;

	// load textures (if any)
	Shader::loadTexture(texPath);
	texLoaded = true;
}


Shader::~Shader()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	if (elementBuffer)
	{
		glDeleteBuffers(1, &EBO);
	}

	glDeleteProgram(ID);

}

// public functions
// ---------------------------------------------------------------------------
void Shader::draw()
{
	glUseProgram(ID);
	glBindVertexArray(VAO);

	if (texLoaded)
	{
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX);
		if (doLogging)
		{
			std::cout 
				<< "BindTexture: texUnit=" << GL_TEXTURE0 
				<< " tex=" << TEX << std::endl;
		}
	}

	if (elementBuffer) 
	{
		glDrawElements(GL_TRIANGLES, geometry.dataLength, GL_UNSIGNED_INT, 0);
		if (doLogging)
		{
			std::cout << "DrawElements: count=" << geometry.dataLength << std::endl;
			doLogging = false;
		}

	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, geometry.dataLength);
		if (doLogging)
		{
			std::cout << "DrawArrays: count=" << geometry.dataLength << std::endl;
			doLogging = false;
		}
	}
}

void Shader::update()
{
	glUseProgram(ID);
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(ID, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}


// shader functions
// ---------------------------------------------------------------------------
std::string Shader::readShaderFile(const char* shader_file)
{
	std::ifstream file(shader_file);
	if (!file) return std::string();

	file.clear();
	file.seekg(0, std::ios_base::beg);

	std::stringstream sstr;
	sstr << file.rdbuf();
	file.close();

	return sstr.str();
}


int Shader::createVertexShader(const char* path)
{
	// read fragment shader source from file at path
	std::string source = Shader::readShaderFile(path);
	const char* shader_source = source.c_str();

	int vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &shader_source, NULL);
	glCompileShader(vid);

	int success;
	char infoLog[512];
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return vid;
}


int Shader::createFragmentShader(const char* path)
{
	// read fragment shader source from file at path
	std::string source = Shader::readShaderFile(path);
	const char* shader_source = source.c_str();

	int fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &shader_source, NULL);
	glCompileShader(fid);

	int success;
	char infoLog[512];
	glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return fid;
}


// geometry functions
// ---------------------------------------------------------------------------
void Shader::loadGeometry(Geometry geo)
{
	// cache the geometry on the shader
	geometry = geo;	
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	if (geo.useIndices)
	{
		
		elementBuffer = true;
		glGenBuffers(1, &EBO);
		std::cout << "Generated Element Buffer " << EBO << std::endl;
	}

	// bind the vertex array
	glBindVertexArray(VAO);

	// bind and set data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, geo.dataSize, geo.data, GL_STATIC_DRAW);
	std::cout << "VBO: size=" << geo.dataSize << std::endl;

	// bind and set element buffers
	if (geo.useIndices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.iSize, geo.indices, GL_STATIC_DRAW);
		std::cout << "EBO: size=" << geo.iSize << std::endl;
	}

	// generate vertex attributes per attribute in source data
	for (int attr = 0; attr < geo.numAttributes; attr++)
	{
		int sum = 0;
		for (int i = 0; i < attr; i++)
		{
			sum += geo.attributes[i];
		}
		
		unsigned int offset = sum * sizeof(float);
		
		glVertexAttribPointer(
			attr, 
			geo.attributes[attr], 
			GL_FLOAT, 
			GL_FALSE, 
			geo.stride, 
			(void*) offset
		);
		glEnableVertexAttribArray(attr);

		std::cout 
			<< "Attribute:"
			<< " attr=" << attr
			<< " sumAttr=" << sum
			<< " lenAttr=" << geo.attributes[attr]
			<< " stride=" << geo.stride 
			<< " offset=" << offset 
		<< std::endl;
	}

	// unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (geo.useIndices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


// texture functions
// ---------------------------------------------------------------------------
void Shader::loadTexture(const char* path)
{
	// if no texture is specified, bail out now
	if (!path) return;
	
	// bind texture
	glGenTextures(1, &TEX);
	glBindTexture(GL_TEXTURE_2D, TEX);

	// set wrapping / filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture at " << path << std::endl;
		stbi_image_free(data);
		return;
	}

	// generate the texture + mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
		width, height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, 
		data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}


// uniform setter functions
// -----------------------------------------------------------------

void Shader::setBool(const std::string& name, bool value) const
{
	glUseProgram(ID);
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUseProgram(ID);
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUseProgram(ID);
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}