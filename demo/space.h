#ifndef SPACE_H
#define SPACE_H

#include "../Utils.h"
#include "../Shader.h"
#include "../Geometry.h"
#include "../Texture.h"
#include "../Camera.h"
#include "../World.h"


inline World* spaceGame(GLFWwindow* window)
{
	Geometry* lightingCube = new Geometry{
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

	MaterialManager* matManager = new MaterialManager();

	//std::string preSchool = "mod/pre_school/v1/pre_school.obj";
	//std::string bulb = "mod/lightbulb/v3/bulb.obj";
	//std::string eye = "mod/eyeball/v1/eyeball.obj";
	//std::string backpack = "mod/backpack/v1/backpack.obj";
	std::string planetPath = "mod/planet/v1/planet.obj";
	std::string rockPath = "mod/rock/v1/rock.obj";

	std::vector<Shader*> objects;

	// Planet Setup
	// --------------------------------------------------------------------------
	Model* planetModel = new Model(planetPath);
	Shader* planet = new Shader(std::map<int, std::string> {
		{ GL_VERTEX_SHADER, "lightObject.vs" },
		{ GL_FRAGMENT_SHADER, "lightObject.fs" }
	}, planetModel);
	planet->name = "planet";

	// planet won't be instanced, so enable normal shader
	planet->createNormalShaderProgram("lightObjectNormal.vs", "lightObjectNormal.fs",
		"lightObjectNormal.gs");

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
	Model* rockModel = new Model(rockPath);
	Shader* rock = new Shader(std::map<int, std::string> {
		{ GL_VERTEX_SHADER, "instancedObject.vs" },
		{ GL_FRAGMENT_SHADER, "lightObject.fs" }
	}, rockModel);
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
	std::vector<LightSource*> lights;
	LightSource* sun = new LightSource(POINT, lightingCube, &matManager->sunlight);
	sun->name = "sun";
	lights.push_back(sun);

	// lights setup
	// TODO: move to function
	for (auto& light : lights)
	{

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

	Camera* camera = new Camera();
	// set up world with camera, objects and lights
	// variables required by objects from lights are also set here
	World* world = new World(window, camera, objects, matManager, lights);

	// world set up
	CubeMap* galaxy = new CubeMap({
		"tex/space/1K/posX.jpg",
		"tex/space/1K/negX.jpg",
		"tex/space/1K/posY.jpg",
		"tex/space/1K/negY.jpg",
		"tex/space/1K/posZ.jpg",
		"tex/space/1K/negZ.jpg" }, "skybox");
	Shader* skybox = new Shader("skybox.vs", "skybox.fs", galaxy);
	skybox->name = "skybox";
	world->skybox = skybox;
	// set skybox sampler to last tex unit + 1
	for (auto& object : world->objects)
	{
		object->setInt(world->skybox->cubeMap->name, object->numTextures);
	}

	// create and add post processing shaders
	Shader* screen = new Shader("postProcessing.vs", "postProcessing.fs");
	world->screen = screen;
	// get the mirror quad roughly the same aspect ratio as screen
	Shader* mirror = new Shader("postProcessing.vs", "postProcessing.fs", 
		0.6f, 0.9f, 0.9f, 0.675f);
	world->mirror = mirror;
	// set up framebuffer and render buffer for post processing
	world->setupPostProcessing();

	return world;
}

#endif SPACE_H
