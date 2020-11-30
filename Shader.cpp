#include "Shader.h"

// construction and deconstruction
// ---------------------------------------------------------------------------
Shader::Shader(
	std::string vertPath, std::string fragPath, 
	Geometry geo, Material mat,
	Texture* textures, unsigned int nTex)
{
	createShaderProgram(vertPath, fragPath);

	// load geometry
	Shader::loadGeometry(geo);
	geometryLoaded = true;

	// load textures (if any)
	if (nTex > 0)
	{
		Shader::loadTextures(textures, nTex);
		texLoaded = true;
	}

	// load materials
	Shader::loadMaterials(mat);
	materialLoaded = true;
}


Shader::Shader(std::string vertPath, std::string fragPath, Model mod)
{
	createShaderProgram(vertPath, fragPath);

	this->mod = mod;
	modelLoaded = true;
}


Shader::Shader(std::string vertPath, std::string fragPath, CubeMap cm)
{
	createShaderProgram(vertPath, fragPath);

	// generate cube geometry
	generateCubeGeometry();

	// load cube map texture
	loadCubeMap(cm);
	cubeMapLoaded = true;
}


Shader::Shader(std::string vertPath, std::string fragPath, std::string geomPath,
	Geometry geo)
{
	createShaderProgram(vertPath, fragPath, geomPath);

	// load geometry
	Shader::loadGeometry(geo);
	geometryLoaded = true;
}


Shader::~Shader() { }


void Shader::tearDown()
{
	if (modelLoaded)
	{
		mod.tearDown();
	}
	else 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		if (elementBuffer)
		{
			glDeleteBuffers(1, &EBO);
		}
	}

	glDeleteProgram(ID);
}


// public functions
// ---------------------------------------------------------------------------
void Shader::draw(GLenum mode)
{	
	
	glUseProgram(ID);
	if (this->doLogging) std::cout << "Drawing Shader " << ID << std::endl;

	if (this->modelLoaded)
	{
		
		if (doLogging) std::cout << "Drawing Opaque Meshes" << std::endl;
		mod.draw(ID);
		if (this->doLogging) this->doLogging = false;
		return;

	}

	// if we're using the old style, keep going	
	if (doLogging) std::cout << "Drawing from Geo, Tex and Materials" << std::endl;
	glBindVertexArray(VAO);

	if (texLoaded)
	{
		for (unsigned int i = 0; i < numTextures; i++)
		{

			useTextureUnit(GL_TEXTURE_2D, i, texList[i].ID);
			if (doLogging)
			{
				std::cout
					<< "BindTexture: texIndex=" << i
					<< " texID=" << texList[i].ID << std::endl;
			}
		}
	}

	if (cubeMapLoaded)
	{
		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.ID);
	}

	if (elementBuffer) 
	{
		glDrawElements(mode, geometry.dataLength, GL_UNSIGNED_INT, 0);
		if (doLogging)
		{
			std::cout << "DrawElements: count=" << geometry.dataLength << std::endl;
		}

	}
	else {
		glDrawArrays(mode, 0, geometry.dataLength);
		if (doLogging)
		{
			std::cout << "DrawArrays: count=" << geometry.dataLength << std::endl;
		}
	}

	if (cubeMapLoaded) glDepthMask(GL_TRUE);

	if (this->doLogging) std::cout << "Finished first draw of shader: " << this->ID << std::endl;
	if (this->doLogging) doLogging = false;
}


void Shader::drawWithOutline()
{
	// draw once normally
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	draw();

	//// draw if not equal to 1 in buffer from previous draw
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);  // don't write to buffer
	glDisable(GL_DEPTH_TEST);

	// draw outline from scaled up container
	glm::mat4 scaledModel = glm::scale(model, glm::vec3(1.1f));
	setMatrix("model", scaledModel);
	setBool("outline", true);
	draw();

	// reset stencil settings
	setBool("outline", false);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);  // disable writing to stencil buffer again
	glEnable(GL_DEPTH_TEST);
}



void Shader::move(const int direction, float deltaTime)
{
	float speed = baseSpeed * deltaTime;

	if (direction == FORWARD)
	{
		setPosition(
			position + 
			(glm::normalize(this->direction) * speed)
		);
	}
	else if (direction == BACKWARD)
	{
		setPosition(
			position -
			(glm::normalize(this->direction) * speed)
		);
	}
	else if (direction == LEFT)
	{
		setPosition(
			position -
			objRight * speed
		);
	}
	else if (direction == RIGHT)
	{
		setPosition(
			position +
			objRight * speed
		);
	}
	else if (direction == DOWN)
	{
		setPosition(
			position +
			objUp * speed
		);
	}
	else if (direction == UP)
	{		
		setPosition(
			position -
			objUp * speed
		);
	}
}


void Shader::translate(float x, float y)
{
	currentTransformation = glm::translate(
		currentTransformation, 
		glm::vec3(x, y, 0.0f)
	);
}


void Shader::rotate(float angle)
{
	currentTransformation = glm::rotate(
		currentTransformation,
		angle,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
}


void Shader::scale(float value)
{
	currentTransformation = glm::scale(
		currentTransformation,
		glm::vec3(value, value, value)
	);
}


void Shader::update()
{
	// deprecated
	Shader::setMatrix("transform", currentTransformation);
	currentTransformation = glm::mat4(1.0f);
}


void Shader::setPosition(glm::vec3 newPostion)
{
	if (!initPositionSet)
	{
		initPositionSet = true;
		initPosition = newPostion;
	}

	position = newPostion;
}


void Shader::setDirection(glm::vec3 newDir)
{
	if (!initDirectionSet)
	{
		initDirectionSet = true;
		initDirection = newDir;
	}
	direction = newDir;
	objRight = glm::normalize(glm::cross(direction, this->worldUp));
	objUp = glm::normalize(glm::cross(objRight, worldUp));
}


glm::mat3 Shader::getNormalMatrix()
{
	return glm::mat3(
		glm::transpose(
			glm::inverse(this->model)
		)
	);
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
void Shader::createShaderProgram(std::string vertPath, std::string fragPath, 
	std::string geomPath)
{
	ID = glCreateProgram();

	int vert_id = createShaderStage(vertPath, GL_VERTEX_SHADER);
	int geom_id;
	if (!geomPath.empty()) geom_id = createShaderStage(geomPath, GL_GEOMETRY_SHADER);
	int frag_id = createShaderStage(fragPath, GL_FRAGMENT_SHADER);

	glAttachShader(ID, vert_id);
	if (!geomPath.empty()) glAttachShader(ID, geom_id);
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

	// tidy up by deleting shaders
	glDeleteShader(vert_id);
	if (!geomPath.empty()) glDeleteShader(geom_id);
	glDeleteShader(frag_id);
}


std::string Shader::readShaderFile(std::string shader_file)
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



int Shader::createShaderStage(std::string path, GLenum type)
{
	// read fragment shader source from file at path
	std::string source = Shader::readShaderFile(path);
	const char* shader_source = source.c_str();

	int vid = glCreateShader(type);
	glShaderSource(vid, 1, &shader_source, NULL);
	glCompileShader(vid);

	std::map<GLenum, std::string> name{
		{GL_VERTEX_SHADER, "VERTEX"},
		{GL_GEOMETRY_SHADER, "GEOMETRY"},
		{GL_FRAGMENT_SHADER, "FRAGMENT"}
	};

	int success;
	char infoLog[512];
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vid, 512, NULL, infoLog);
		std::cout 
			<< "ERROR::SHADER::" 
			<< name[type] 
			<<"::COMPILATION_FAILED\n" 
			<< infoLog 
			<< std::endl;
	}

	return vid;
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


void Shader::generateCubeGeometry()
{
	std::cout << "Generating cube" << std::endl;
	
	std::vector<glm::vec3> vertices = {
		glm::vec3( 1,  1,  1),  // back top right      0
		glm::vec3(-1,  1,  1),  // back top left       1
		glm::vec3(-1, -1,  1),  // back bottom left    2
		glm::vec3( 1, -1,  1),  // back bottom right   3
		glm::vec3(-1,  1, -1),  // front top left      4
		glm::vec3( 1,  1, -1),  // front top right     5
		glm::vec3( 1, -1, -1),  // front bottom right  6
		glm::vec3(-1, -1, -1)   // front bottom left   7
	};

	std::vector<glm::vec4> faces = {
		glm::vec4(4, 7, 6, 5),  // front
		glm::vec4(1, 2, 7, 4),  // left
		glm::vec4(5, 6, 3, 0),  // right
		glm::vec4(0, 3, 2, 1),  // back
		glm::vec4(1, 4, 5, 0),  // top
		glm::vec4(7, 2, 3, 6),  // bottom
	};

	std::vector<int> indexOrder = { 
		0, 1, 2,  // bottom left triangle
		2, 3, 0   // top right triangle
	};

	Geometry geo;
	geo.numAttributes = 1;
	geo.attributes[0] = 3;
	geo.stride = geo.attributes[0] * sizeof(float);

	// add unique vertices
	int k = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			geo.data[k] = vertices[i][j];
			k++;
		}
	}
	geo.dataLength = indexOrder.size() * faces.size();
	geo.dataSize = k * geo.attributes[0] * sizeof(float);

	// add indices
	geo.useIndices = true;
	geo.iLength = 0;
	for (int i = 0; i < faces.size(); i++)
	{
		for (int j : indexOrder)
		{
			geo.indices[geo.iLength] = static_cast<int>(faces[i][j]);
			geo.iLength++;
		}
	}
	geo.iSize = geo.iLength * sizeof(int);

	this->geometry = geo;
	loadGeometry(geo);
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


void Shader::loadCubeMap(CubeMap cubeMap)
{
	glUseProgram(ID);
	setInt(cubeMap.name, 0);
	this->cubeMap = cubeMap;
	std::cout
		<< "Assigned samplerCube to texture unit"
		<< " name=" << cubeMap.name
		<< " ID=" << cubeMap.ID
		<< std::endl;
}


void Shader::useTextureUnit(GLenum target, unsigned int texUnitOffset, unsigned int texID)
{
	glUseProgram(ID);
	
	// activate texture unit in order of array
	glActiveTexture(GL_TEXTURE0 + texUnitOffset);
	// bind the activated texture unit
	glBindTexture(target, texID);
}


void Shader::loadMaterials(Material mat) 
{
	material = mat;

	ambient = Attribute<glm::vec3>(
		material.ambient, glm::vec3(0.0f), glm::vec3(1.0f),
		glm::vec3(0.05f), LINEAR_SCALE
		);
	diffuse = Attribute<glm::vec3>(
		material.diffuse, glm::vec3(0.0f), glm::vec3(1.0f),
		glm::vec3(0.05f), LINEAR_SCALE
		);
	specular = Attribute<glm::vec3>(
		material.specular, glm::vec3(0.0f), glm::vec3(1.0f),
		glm::vec3(0.05f), LINEAR_SCALE
		);
	shininess = Attribute<float>(material.shininess * 256, 2.0f, 256.0f, 2.0f, EXPONENTIAL_SCALE);
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


void Shader::setMatrix(const std::string& name, glm::mat3 value) const
{
	glUseProgram(ID);
	unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix3fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUseProgram(ID);
	unsigned int vecLoc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}


void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUseProgram(ID);
	unsigned int vecLoc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

// light source functions
// ---------------------------------------------------------------------------


void LightSource::setAttenuation(int index)
{
	attenuationIndex = index;

	distance = attenuation[index].x;
	constant = attenuation[index].y;
	linear = attenuation[index].z;
	quadratic = attenuation[index].w;
}


std::string LightSource::uniform(int i, std::string name)
{
	return "lights[" + std::to_string(i) + "]." + name;
}


// Screen Shader functions
// ---------------------------------------------------------------------------

Shader::Shader(std::string vertPath, std::string fragPath, float x1, float y1, float x2, float y2)
{
	Shader::createShaderProgram(vertPath, fragPath);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	static const int DLEN = 24;
	static const int DSIZE = DLEN * sizeof(float);
	// create data in device coordinates
	float data[DLEN] = {
		// geo    tex
		// triangle 1
		x1, y1,   0.0f,  1.0f,
		x1, y2,   0.0f,  0.0f,
		x2, y2,   1.0f,  0.0f,
	    // triangle 2
		x1, y1,   0.0f,  1.0f,
		x2, y2,   1.0f,  0.0f,
		x2, y1,   1.0f,  1.0f
	};

	// bind and set data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, DSIZE, &data, GL_STATIC_DRAW);

	// generate vertex attributes for geometry
	int aSize = 2 * sizeof(float);  // size of each attribute in bytes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, aSize*2, (void*)0);
	// and again for texture
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, aSize*2, (void*)aSize);

	// unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// set texture uniform
	glUseProgram(ID);
	glUniform1i(glGetUniformLocation(ID, "screenTexture"), 0);
}


void Shader::screenDraw(unsigned int tcb)
{
	glUseProgram(ID);
	glBindVertexArray(VAO);

	// bind the activated texture unit
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tcb);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}