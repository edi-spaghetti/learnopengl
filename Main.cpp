#include <iostream>
#include <fstream>
#include <sstream>

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
	1,
    {3},

	3 * sizeof(float),
	36,
	36 * 3 * sizeof(float),
	{
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
	},
	false
};

Geometry litCube = {
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


int main()
{

	// guess what this does
	int width = 800;
	int height = 600;
	GLFWwindow* window = createWindow(width, height);

	glEnable(GL_DEPTH_TEST);

	MaterialManager matManager = MaterialManager();

	// construct shaders and load geometry and textures
	Shader objectShader = Shader("lightObject.vs", "lightObject.fs", litCube, matManager.emerald);
	Shader lightSourceShader = Shader("lightSource.vs", "lightSource.fs", lightingCube, Material());

	Camera camera = Camera();
	// set up world with camera, objects and lights
	// variables required by objects from lights are also set here
	World world = World(window, &camera, &objectShader, &lightSourceShader);

	// setup user controls
	setupUserControls(window, &world);

	// global lighting position vector
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	// instantiate container for updated light position it moving it
	glm::vec3 newLightPos;

	// start render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		world.update();

		// render
		// set the colour to black-ish
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// move light source to another location and draw
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, world.getLightRotationAngle(), glm::vec3(0.0, 1.0, 0.0));
		
		// recalculate light position after rotation
		// do this before adding world space translation because lightPos
		// already is in world space, so multiplying the model after adding 
		// translation would apply the translation twice
		if (world.shadeInViewSpace)
		{
			newLightPos = glm::vec3(
				world.view * model * glm::vec4(lightPos, 1.0f)
			);
		}
		else
		{

			newLightPos = glm::vec3(model * glm::vec4(lightPos, 1.0f));
		}
		
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSourceShader.setMatrix("model", model);
		lightSourceShader.draw();

		// set model matrix for light source at id matrix
		model = glm::mat4(1.0f);
		objectShader.setMatrix("model", model);
		objectShader.setVec3("light.position", newLightPos);

		if (world.shadeInViewSpace)
		{
			objectShader.setVec3("viewPos", glm::vec3(0.0f));
			// generate normal matrix from model-view matrix to get in view space
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(world.view * model));
			objectShader.setMatrix("normalMatrix", normalMatrix);
		}
		else
		{
			objectShader.setVec3("viewPos", world.camera->position);
			// generate normal matrix from model matrix
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
			objectShader.setMatrix("normalMatrix", normalMatrix);
		}
		
		objectShader.draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	glfwTerminate();
	return 0;
}
