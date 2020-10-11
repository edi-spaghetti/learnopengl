#include "Shader.h"


Shader::Shader(const char* vertPath, const char* fragPath, Geometry geo)
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


void Shader::draw()
{
	glUseProgram(ID);
	
	if (elementBuffer) {

	}
	else {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, geometry.dataLength);
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
// -----------------------------------------------------------------
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
	}

	// bind the vertex array
	glBindVertexArray(VAO);

	// bind and set data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, geo.dataSize, geo.data, GL_STATIC_DRAW);

	// bind and set element buffers
	if (geo.useIndices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.iSize, geo.indices, GL_STATIC_DRAW);
	}

	// generate vertex attributes per attribute in source data
	for (int attr = 0; attr < geo.numAttributes; attr++)
	{
		unsigned int offset = geo.attributes[attr] * attr * sizeof(float);
		
		glVertexAttribPointer(
			attr, 
			geo.attributes[attr], 
			GL_FLOAT, 
			GL_FALSE, 
			geo.stride, 
			(void*) offset
		);
		glEnableVertexAttribArray(attr);
	}

	// unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (geo.useIndices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


// uniform setter functions
// -----------------------------------------------------------------

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}