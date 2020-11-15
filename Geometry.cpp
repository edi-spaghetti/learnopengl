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


Mesh::~Mesh() { }


void Mesh::tearDown()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	// bind the vertex array
	glBindVertexArray(this->VAO);
	// bind and set data into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vSize, &this->vertices[0], GL_STATIC_DRAW);

	// bind and set element buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->iSize, &this->indices[0], GL_STATIC_DRAW);

	std::cout << "Generated VBO " << this->vSize
		<< " EBO " << this->iSize
		<< std::endl;

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


void Mesh::draw() {

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


void Model::tearDown()
{
	for (Mesh mesh : meshes)
	{
		mesh.tearDown();
	}
}


void Model::draw()
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
		meshes[i].draw();
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\/'));
	std::cout << "directory=" << directory << std::endl;

	processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode* node, const aiScene* scene)
{

	// prcoess all node's meshes if it has any
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// now recursively drill down
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{

	// process verticies
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		// position vectors

		for (int j = 0; j < 3; j++)
			vector[j] = mesh->mVertices[i][j];
		vertex.position = vector;

		// indices
		if (mesh->HasNormals())
		{
			for (int j = 0; j < 3; j++)
				vector[j] = mesh->mNormals[i][j];
			vertex.normal = vector;
		}

		// texture coords
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = tex;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// process indices
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process textures
	std::vector<Texture> textures;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// diffuse
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
			aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// specular
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,
	aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			std::string srcPath = textures_loaded[j].path;
			std::string trgPath = directory + '/' + str.C_Str();
			std::cout << "Comparing " << srcPath << " & " << trgPath << std::endl;
			bool match = srcPath.compare(trgPath) == 0;
			if (match)
			{
				std::cout << "Loading existing texture" << std::endl;
				// if matched path, texture is already loaded, so add it to list
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			// initialise a new texture at path
			std::string path = directory + '/' + str.C_Str();
			std::cout << "Loading new texture at path " << path << std::endl;
			Texture texture = Texture(path, typeName.c_str());
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
			
		
	}

	return textures;
}
