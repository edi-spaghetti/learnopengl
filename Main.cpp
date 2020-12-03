#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "Utils.h"
#include "Shader.h"
#include "Geometry.h"
#include "Texture.h"
#include "Camera.h"
#include "World.h"


Geometry jamal = {
	// attributes
	2,
	{3, 3},

	// stride
	6 * sizeof(float),

	// data
	3,
	18 * sizeof(float),
	{
	// position             // colours
	-1.0f , 0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
	-0.5f, 1.0f, 0.0f,      0.0f,  1.0f,  0.0f,
	 0.0f , 0.0f, 0.0f,     0.0f,  0.0f,  1.0f
	},

	// elements
	false,
	0,
	{}
};

Geometry persephone = {
	// attributes
	1,
	{3},

	// stride
	3 * sizeof(float),

	// data
	6,
	12 * sizeof(float),
	{
		 0.5f,  0.5f, 0.0f,  // top right = 0
		 0.5f, -0.5f, 0.0f,  // bottom right = 1
		-0.5f, -0.5f, 0.0f,  // bottom left = 2
		-0.5f,  0.5f, 0.0f,  // top left = 3
	},

	// elements
	true,
	6,
	6 * sizeof(int),
    {
	    0, 1, 2,
		0, 2, 3
    }
};

Geometry karen = {
	// attributes
	3,
	{3, 3, 2},

	// stride
	8 * sizeof(float),

	// data
	6,
	32 * sizeof(float),
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right = 0
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right = 1
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left = 2
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left = 3
    },

	// elements
	true,
	6,
	6 * sizeof(int),
	{
		0, 1, 3,
		1, 2, 3
	}
};

Geometry balthazar = {
    // attributes
	2,
    { 3, 2 },

	// stride
	5 * sizeof(float),

	// data
	36,
	36 * 5 * sizeof(float),
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	},
	false
};

Geometry lightingCube = {
	2,
	{3, 3},

	6 * sizeof(float),
	36,
	36 * 6 * sizeof(float),
	{
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

	},
	false
};

Geometry litCube = {
	3,
	{3, 3, 2},

	8 * sizeof(float),
	36,
	36 * 8 * sizeof(float),
	{
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	},
	false
};


int main()
{
	std::cout << "Starting..." << std::endl;

	// guess what this does
	int width = 800;
	int height = 600;
	GLFWwindow* window = createWindow(width, height);

	// global opengl settings
	// -----------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MaterialManager matManager = MaterialManager();

	//const int nTextures = 3;
	//Texture textures[nTextures] = {
	//	Texture("container2.png", "material.diffuse"),
	//	Texture("container2_specular.png", "material.specular"),
	//	Texture("container2_animated_emission.png", "material.emission", true, GL_REPEAT, GL_NEAREST)
	//};

	// //construct shaders and load geometry and textures
	//Shader objectShader = Shader(
	//	"lightObject.vs", "lightObject.fs", 
	//	litCube, 
	//	matManager.emerald,
	//	textures, nTextures
	//);

	//std::string preSchool = "mod/pre_school/v1/pre_school.obj";
	//std::string bulb = "mod/lightbulb/v3/bulb.obj";
	//std::string eye = "mod/eyeball/v1/eyeball.obj";
	//std::string backpack = "mod/backpack/v1/backpack.obj";
	std::string planetPath = "mod/planet/v1/planet.obj";
	std::string rockPath = "mod/rock/v1/rock.obj";

	std::vector<Shader*> objects;

	// Planet Setup
	// --------------------------------------------------------------------------
	Shader* planet = new Shader(std::map<int, std::string> { 
		{ GL_VERTEX_SHADER,   "lightObject.vs" }, 
		{ GL_FRAGMENT_SHADER, "lightObject.fs" }
	}, Model(planetPath));
	planet->name = "planet";

	// set planet's starting position in world space
	glm::vec2 orbit = glm::circularRand(10.0f);
	float randY = glm::linearRand(-0.5f, 0.5f);
	glm::vec3 initPos = glm::vec3(orbit[0], randY, orbit[1]);
	planet->setPosition(initPos);
	printf("Planet Init Position %f %f %f\n", orbit.x, randY, orbit.y);

	// set a random spin
	glm::vec2 pSpin = glm::circularRand(1);
	planet->setDirection(glm::vec3(pSpin[0], 0, pSpin[1]));

	// add to the vector to be added to world
	objects.push_back(planet);


	// Rock Setup
	// --------------------------------------------------------------------------
	Shader* rock = new Shader(std::map<int, std::string> {
		{ GL_VERTEX_SHADER, "instancedObject.vs" },
		{ GL_FRAGMENT_SHADER, "lightObject.fs" }
	}, Model(rockPath));
	rock->name = "rock";

	// configure positions and instancing
	unsigned int instances = 500;
	std::srand(glfwGetTime());
	float radius = 25.0f;
	float offset = 2.5f;

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::mat3> normalMatrices;
	for (unsigned int i = 0; i < instances; i++)
	{
		// set at random orbit around sun
		orbit = glm::circularRand(radius);
		glm::vec3 randOffset = glm::sphericalRand(offset);
		initPos = glm::vec3(
			orbit[0] + randOffset.x, 
			randY + randOffset.y, 
			orbit[1] + randOffset.z
		);
		rock->setPosition(initPos);
		printf("Rock Init Position %s\n", glm::to_string(initPos).c_str());

		// set up a random direction
		glm::vec3 spin = glm::sphericalRand(1.0f);
		rock->setDirection(spin);
		printf("Rock Init Direction %s\n", glm::to_string(spin).c_str());

		// set a random size
		float size = glm::linearRand(0.1f, 0.75f);
		rock->size = glm::vec3(size);
		printf("Rock initialised at %.2fx size\n", size);

		// keep track of matrices per instance
		glm::mat4 model = rock->genModelMatrix();
		rock->model = model;
		glm::mat3 normal = rock->getNormalMatrix();
		modelMatrices.push_back(model);
		normalMatrices.push_back(normal);
		rock->instances++;
	}
	rock->addInstancedVertexAttribute(modelMatrices, 1, 3);
	rock->addInstancedVertexAttribute(normalMatrices, 1, 7);
	objects.push_back(rock);

	// add objects to list, to later be added to world on initialisation

	//Shader objectShader = Shader(
	//	"lightObject.vs", "lightObject.fs", Model(backpack)
	//);

	//Shader instanceShader = Shader(
	//	std::map<int, std::string> {
	//		{ GL_VERTEX_SHADER, "instanceTest.vs" },
	//		{ GL_FRAGMENT_SHADER, "instanceTest.fs" }
	//	},
	//	Geometry{
	//		2, {2, 3}, 20, 6, 120,
	//		{    // positions     // colors
	//			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	//			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	//			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

	//			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	//			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	//			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	//		}, false
	//	}
	//);

	//std::vector<glm::vec2> translations;
	//float offset = 0.1f;
	//for (int y = -10; y < 10; y += 2)
	//{
	//	for (int x = -10; x < 10; x += 2)
	//	{
	//		glm::vec2 t;
	//		t.x = (float)x / 10.0f + offset;
	//		t.y = (float)y / 10.0f + offset;
	//		
	//		translations.push_back(t);
	//	}
	//}

	//instanceShader.addInstancedVertexAttribute(translations, 1);

	//Shader objectShader = Shader(
	//	"geomShaderTest.vs", "geomShaderTest.fs", "geomShaderTest.gs",
	//	Geometry{ 2, {2, 3}, 20, 4, 80,
	//	{
	//		-0.5f,  0.5f, 1, 0, 0,
	//		 0.5f,  0.5f, 0, 1, 0,
	//		 0.5f, -0.5f, 0, 0, 1,
	//		-0.5f, -0.5f, 1, 1, 0
	//	}, false }
	//	//Geometry{ 2, {2, 3}, 20, 1, 20, {0, 0, 0, 1, 0}, false }
	//);

	//objectShader.size *= 0.1f;

	std::vector<LightSource*> lights;
	LightSource* sun = new LightSource(POINT, lightingCube, matManager.sunlight);
	sun->name = "sun";
	lights.push_back(sun);

	// lights setup
	// TODO: move to function
	for (auto &light : lights)
	{
		// set a random height, and make the directional light much higher
		//float yPos = glm::linearRand(0.0f, 2.0f);
		//if (light.type == DIRECTIONAL) yPos += 3.0f;

		// set light's starting position in world space
		light->setPosition(glm::vec3(0, 0, 0));

		// set up a lighting direction
		light->setDirection(
			glm::vec3(
				glm::linearRand(-0.2f, 0.2f),
				0,
				glm::linearRand(-0.3f, 0.3f)
			)
		);

		// bump up attenutation to max
		light->setAttenuation(11);
	}

	Camera camera = Camera();
	//// set up world with camera, objects and lights
	//// variables required by objects from lights are also set here
	World world = World(window, &camera, objects, &matManager, lights);

	// world set up
	world.skybox = Shader("skybox.vs", "skybox.fs", CubeMap({
		"tex/space/1K/posX.jpg",
		"tex/space/1K/negX.jpg",
		"tex/space/1K/posY.jpg",
		"tex/space/1K/negY.jpg",
		"tex/space/1K/posZ.jpg",
		"tex/space/1K/negZ.jpg" }, "skybox"));
	world.skybox.name = "skybox";
	// set skybox sampler to last tex unit + 1
	for (auto& object : world.objects)
	{
		object->setInt(world.skybox.cubeMap.name, object->numTextures);
	}

	// setup user controls
	setupUserControls(window, &world);
	bool doLogging = true;
	// start render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		world.update();

		// render
		world.draw();
		//objectShader.draw(GL_POINTS);
		//glUseProgram(instanceShader.ID);
		//glBindVertexArray(instanceShader.VAO);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 
		//	instanceShader.geometry.dataLength, translations.size());
		//if (doLogging)
		//{
		//	printf("Drawing triangles from %d vertices x %d instances",
		//		instanceShader.geometry.dataLength, translations.size());
		//	doLogging = false;
		//}
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	//instanceShader.tearDown();
	world.screen.tearDown();
	//objectShader.tearDown();
	for (auto& object : objects)
	{
		printf("Cleaning up object shader %d\n", object->ID);
		object->tearDown();
	}
	for (auto& light : lights) 
	{
		std::cout << "Cleaning Up Shader " << light->ID << std::endl;
		light->tearDown();
	}


	glfwTerminate();
	return 0;
}
