#include "World.h"


World::World(GLFWwindow* win, Camera* cam, Shader* obj, Shader* lgt, MaterialManager* mng)
{

	// TODO: multiple objects + layers
	//       this will allow rendering geo to signify light source's position
	//       without affecting how light hits the other objects / itself

	window = win;
	camera = cam;
	object = obj;
	light = lgt;
	matManager = mng;

	camera->world = this;

	// initialise object material uniforms
	if (!object->texLoaded)
	{
		object->setVec3("material.ambient", object->ambient.value);
		object->setVec3("material.diffuse", object->diffuse.value);
	}

	object->setVec3("material.specular", object->specular.value);
	object->setFloat("material.shininess", object->shininess.value);

	// initalise light properties on objects
	object->setVec3("light.ambient", glm::vec3(light->ambient.value));
	object->setVec3("light.diffuse", glm::vec3(light->diffuse.value));
	object->setVec3("light.specular", glm::vec3(light->specular.value));

	if (light->materialLoaded)
	{
		for (int i = 0; i < matManager->numMaterials; i++)
		{
			if (matManager->materials[i] == &(light->material))
			{
				matManager->currentMaterial = i;
				break;
			}
		}
	}

}

World::~World()
{

}


void World::update()
{
	World::updateTime();
	World::updateScreen();
	World::updateAttributes();
	World::updateView();
	World::updateProjection();
	World::updateModel();
}


void World::updateTime()
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

}

void World::updateScreen()
{
	processInput(window, &screenWidth, &screenHeight);
}


void World::updateAttributes()
{
	// TODO: dynamically determine which attributes need to be set
	object->setVec3("light.ambient", glm::vec3(light->ambient.value));
	object->setVec3("light.diffuse", glm::vec3(light->diffuse.value));
	object->setVec3("light.specular", glm::vec3(light->specular.value));
	object->setFloat("material.shininess", object->shininess.value);

	// set the light uniforms to draw the light cube from it's currently 
	// loaded material
	light->setVec3("light.ambient", glm::vec3(light->ambient.value));
	light->setVec3("light.diffuse", glm::vec3(light->diffuse.value));
	light->setVec3("light.specular", glm::vec3(light->specular.value));
	light->setFloat("light.shininess", light->shininess.value);

	// TODO: support for other (e.g. boolean) attributes
	object->setBool("shadeInViewSpace", shadeInViewSpace);
	object->setInt("toggleGouraudPhong", int(toggleGouraudPhong));
}


// creates a view matrix from camera and sets it on the object shader
void World::updateView()
{
	// move back (= scene forwards) with the view matrix
	view = glm::lookAt(
		camera->position,
		camera->position + camera->front,
		camera->up
	);

	object->setMatrix("view", view);
	light->setMatrix("view", view);
}


void World::updateProjection()
{
	projection = glm::perspective(
		// fov
		glm::radians(camera->FOV),
		// aspect ratio
		((float) screenWidth * 1) / ((float) screenHeight * 1),
		// near and far clipping planes
		0.1f, 100.0f
	);

	object->setMatrix("projection", projection);
	light->setMatrix("projection", projection);
}


void World::updateModel()
{
	// leave blank for now
}


float World::getLightRotationAngle()
{
	if (staticLight) return glm::radians(currentLightRotationAngle);

	currentLightRotationAngle += 0.05f;
	return glm::radians(currentLightRotationAngle);
}


void World::cycleMaterial(bool forward)
{
	if (forward)
	{
		matManager->currentMaterial = (matManager->numMaterials + 1) % matManager->numMaterials;
	}
	else
	{
		if (matManager->currentMaterial)
		{
			matManager->currentMaterial -= 1;
		}
		else
		{
			matManager->currentMaterial = matManager->numMaterials - 1;
		}
	}

	light->loadMaterials(*(matManager->materials[matManager->currentMaterial]));
}