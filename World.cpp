#include "World.h"


World::World(GLFWwindow* win, Camera* cam, Shader* shad)
{
	window = win;
	camera = cam;
	shader = shad;
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
	// TODO: dynamic attributes
	shader->setFloat("ambientStrength", shader->ambient.value);
	shader->setFloat("specularStrength", shader->specular.value);
	shader->setFloat("shininessPower", shader->shininess.value);
	// TODO: support for other (e.g. boolean) attributes
	shader->setBool("shadeInViewSpace", shadeInViewSpace);
	shader->setInt("toggleGouraudPhong", int(toggleGouraudPhong));
}


// creates a view matrix from camera and sets it on the shader
void World::updateView()
{
	// move back (= scene forwards) with the view matrix
	view = glm::lookAt(
		camera->position,
		camera->position + camera->front,
		camera->up
	);

	shader->setMatrix("view", view);
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
	shader->setMatrix("projection", projection);
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
