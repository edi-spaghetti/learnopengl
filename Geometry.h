#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"


const int MAX_DATA_SIZE = 360;
const int MAX_ATTRIBUTES = 3;


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct VertexAttributes {
	int numAttributes = 3;
	std::vector<int> attrSizes = { 3, 3, 2 };
};


//struct Texture {
//	unsigned int id;
//	std::string type;
//};


class Mesh {
public:
	// main data attributes
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// support data attributes
	int vSize;
	int iSize;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
		std::vector<Texture> textures);
	~Mesh();

	void draw(Shader &shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
	bool doLogging = true;
};


struct Geometry {
	int numAttributes;
	int attributes[MAX_ATTRIBUTES];

	int stride;  // number of bytes per vertex = sum(attributes) * sizeof(float)

	unsigned int dataLength;  // number of active vertices in the data array
	unsigned int dataSize;  // number active elements multiplied by sizeof(float)
	float data[MAX_DATA_SIZE];

	bool useIndices;
	unsigned int iLength;
	unsigned int iSize;  // number of active elements in indices mutiplied by sizeof(int)
	unsigned int indices[MAX_DATA_SIZE];
};

#endif
