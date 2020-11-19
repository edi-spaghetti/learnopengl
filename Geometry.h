#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	float opacity;
	glm::vec3 localPosition;

	// support data attributes
	int vSize;
	int iSize;

	Mesh() {};
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
		std::vector<Texture> textures);
	~Mesh();
	void tearDown();
	void setOpacity(float opacity) { this->opacity = opacity; };
	void setLocalPosition(glm::vec3 position) { this->localPosition = position; };

	void draw(unsigned int ID);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
	bool doLogging = true;
};


class Model {
public: 
	Model() {};
	Model(std::string path) {
		loadModel(path);
	};
	void tearDown();
	void draw(unsigned int ID);
	const std::vector<Mesh> getTransparentMeshes() { return transparentMeshes; };
private:
	std::vector<Mesh> meshes;
	std::vector<Mesh> transparentMeshes;
	std::vector<Texture> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, 
		aiTextureType type, std::string typeName);
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
