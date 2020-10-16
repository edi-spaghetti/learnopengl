#include "Shader.h"

// construction and deconstruction
// ---------------------------------------------------------------------------
Shader::Shader(
	const char* vertPath, const char* fragPath, 
	Geometry geo, Texture* textures, unsigned int nTex)
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
	Shader::loadTextures(textures, nTex);
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
		for (unsigned int i = 0; i < numTextures; i++)
		{

			// activate texture unit in order of array
			glActiveTexture(GL_TEXTURE0 + i);

			// bind the activated texture unit
			glBindTexture(GL_TEXTURE_2D, texList[i].ID);
			if (doLogging)
			{
				std::cout
					<< "BindTexture: texIndex=" << i
					<< " texID=" << texList[i].ID << std::endl;
			}
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
	glm::mat4 trans = glm::mat4(1.0f);
	// rotate by                 this much               around the Z-axis
	trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
	// move to bottom right
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	Shader::setMatrix("transform", trans);
}


void Shader::increaseTransparency()
{
	if (currentAlpha < maxAlpha)
	{
		currentAlpha += alphaIncrement;
	}

	// if less than one increment away from max, set to max
	// this avoids floating point precision problems
	if ((maxAlpha - alphaIncrement) < currentAlpha) 
	{ 
		currentAlpha = maxAlpha; 
	}

	Shader::setFloat("alpha", currentAlpha);
	std::cout << "currentAlpha=" << currentAlpha << std::endl;
}


void Shader::decreaseTransparency()
{
	if (currentAlpha > minAlpha)
	{
		currentAlpha -= alphaIncrement;
	}

	// if less than one increment away from minimum, set to minimum
	// this avoids floating point precision problems
	if (currentAlpha < (minAlpha + alphaIncrement))
	{
		currentAlpha = minAlpha;
	}

	Shader::setFloat("alpha", currentAlpha);
	std::cout << "currentAlpha=" << currentAlpha << std::endl;
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


void Shader::loadTextures(Texture* textures, unsigned int nTex)
{
	numTextures = nTex;
	texList = textures;
	
	glUseProgram(ID);

	for (unsigned int i = 0; i < nTex; i++)
	{
		Shader::setInt(textures[i].name, i);

		std::cout 
			<< "Assigned sampler to texture unit"
			<< " name=" << textures[i].name 
			<< " ID=" << textures[i].ID 
		<< std::endl;
	};

	Shader::setFloat("zoom", 0.0f);
	std::cout << "Initialised zoom to 0.0f" << std::endl;

	Shader::setFloat("alpha", currentAlpha);
	std::cout << "Initalised alpha to " << currentAlpha << std::endl;
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

void Shader::setMatrix(const std::string& name, glm::mat4 value) const
{
	glUseProgram(ID);
	unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}