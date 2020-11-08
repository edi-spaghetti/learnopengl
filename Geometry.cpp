#include "Geometry.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
	std::vector<Texture> textures) 
{
	// cache main data from params
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// calculate any helper values
	this->vSize = vertices.size() * sizeof(Vertex);
	this->iSize = indices.size() * sizeof(unsigned int);

	setupMesh();
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	std::cout << "Deleted Buffers and Vertex Arrays" << std::endl;
}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	std::cout << "Generated Buffers and Vertex Arrays" << std::endl;

	// bind the vertex array
	glBindVertexArray(this->VAO);
	// bind and set data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vSize, &this->vertices[0], GL_STATIC_DRAW);
	std::cout << "VBO: size=" << this->vSize << std::endl;

	// bind and set element buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->iSize, &this->indices[0], GL_STATIC_DRAW);
	std::cout << "EBO: size=" << this->iSize << std::endl;

	// generate vertex attributes per attribute in source data
	VertexAttributes va;
	for (int i = 0; i < va.numAttributes; i++)
	{
		int sum = 0;
		for (int j = 0; j < i; j++)
		{
			sum += va.attrSizes[j];
		}

		unsigned int offset = sum * sizeof(float);

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i, va.attrSizes[i], 
			GL_FLOAT, GL_FALSE, 
			sizeof(Vertex), (void*)offset
		);
	}

	// unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Mesh::draw(Shader &shader) {

	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < this->textures.size(); i++)
	{

		// activate texture unit in order of array
		glActiveTexture(GL_TEXTURE0 + i);

		// bind the activated texture unit
		glBindTexture(GL_TEXTURE_2D, this->textures[i].ID);
		if (doLogging) std::cout
				<< "BindTexture: texIndex=" << i
				<< " texID=" << this->textures[i].ID << std::endl;
	}

	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	if (this->doLogging) std::cout << "DrawElements: count=" 
		<< this->indices.size() << std::endl;

	if (this->doLogging) std::cout << "Finished first draw of mesh" 
		<< std::endl;
	doLogging = false;
}