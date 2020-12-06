#ifndef STREET_H
#define STREET_H

#include "../Utils.h"
#include "../Shader.h"
#include "../Geometry.h"
#include "../Texture.h"
#include "../Camera.h"
#include "../World.h"


inline World* streetDemo(GLFWwindow* window)
{
	// Objects
	// --------------------------------------------------------------------------
	std::vector<Shader*> objects;
	std::string streetPath = "mod/crossroad/v1/street.obj";
	Model* streetModel = new Model(streetPath);
	Shader* street = new Shader(std::map<int, std::string> {
		{ GL_VERTEX_SHADER, "lightObject.vs" },
		{ GL_FRAGMENT_SHADER, "lightObject.fs" }
	}, streetModel);
	street->name = "street";

	street->setPosition(glm::vec3(0, -3, 0));
	street->setDirection(glm::vec3(0, 0, -1));

	// add to object list
	objects.push_back(street);

	// Lights
	// --------------------------------------------------------------------------
	MaterialManager* matManager = new MaterialManager();
	std::vector<LightSource*> lights;
	// TODO: procedurally generate sphere
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
	LightSource* sun = new LightSource(POINT, lightingCube, &matManager->sunlight);
	sun->name = "sun";
	// set light's starting position in world space
	sun->setPosition(glm::vec3(0, 10, 0));

	// set up a lighting direction
	sun->setDirection(
		glm::vec3(
			glm::linearRand(-0.2f, 0.2f),
			0,
			glm::linearRand(-0.3f, 0.3f)
		)
	);

	// bump up attenutation to max
	sun->setAttenuation(11);
	
	// add to list
	lights.push_back(sun);

	// World Setup
	// --------------------------------------------------------------------------
	Camera* camera = new Camera();
	World* world = new World(window, camera, objects, matManager, lights);

	// SkyBox
	// --------------------------------------------------------------------------
	CubeMap* galaxy = new CubeMap({
	"tex/space/1K/posX.jpg",
	"tex/space/1K/negX.jpg",
	"tex/space/1K/posY.jpg",
	"tex/space/1K/negY.jpg",
	"tex/space/1K/posZ.jpg",
	"tex/space/1K/negZ.jpg" }, "skybox");
	Shader* skybox = new Shader("skybox.vs", "skybox.fs", galaxy);
	skybox->name = galaxy->name;
	world->skybox = skybox;
	// set skybox sampler to last tex unit + 1
	for (auto& object : world->objects)
	{
		object->numTextures = 15;
		object->setInt(world->skybox->cubeMap->name, object->numTextures);
	}

	// Post Processing
	// --------------------------------------------------------------------------
	// create and add post processing shaders
	Shader* screen = new Shader("postProcessing.vs", "postProcessing.fs");
	world->screen = screen;
	// get the mirror quad roughly the same aspect ratio as screen
	Shader* mirror = new Shader("postProcessing.vs", "postProcessing.fs",
		0.6f, 0.9f, 0.9f, 0.675f);
	world->mirror = mirror;
	// set up framebuffer and render buffer for post processing
	world->setupPostProcessing();

	// done!
	return world;
}


#endif STREET_H