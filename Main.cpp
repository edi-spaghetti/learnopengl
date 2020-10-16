#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.h"
#include "Shader.h"
#include "Geometry.h"
#include "Texture.h"


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


int main()
{

	// guess what this does
	int width = 800;
	int height = 600;
	GLFWwindow* window = createWindow(width, height);

	// construct shaders and load geometry and textures
	//Shader jShader = Shader("colourShape.vs", "colourShape.fs", jamal);
	//Shader pShader = Shader("default.vs", "default.fs", persephone);
	int nTextures = 2;
	Texture textures[] = { 
		Texture("container.jpg", false, false, GL_CLAMP_TO_EDGE, GL_NEAREST), 
		Texture("awesomeface.png", true, true) 
	};

	Shader kShader = Shader("CoordSystems.vs", "posColTex.fs", karen, textures, nTextures);
	//kShader.setInt("ourTexture", 0);

	glfwSetWindowUserPointer(window, &kShader);
	glfwSetKeyCallback(window, key_callback);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Max number vertex attributes supported: " << nrAttributes << std::endl;

	float location = 0.0f;
	float velocity = 0.0001f;

	// start render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window, &width, &height);

		// render
		// set the colour to pleasant green
		glClearColor(0.0f, 0.6f, 0.029f, 1.0f);
		// fill er up
		glClear(GL_COLOR_BUFFER_BIT);

		location += velocity;
		//jShader.setFloat("vLocation", location);

		// rotate backwards with model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		kShader.setMatrix("model", model);
		// move back (= scene forwards) with the view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		kShader.setMatrix("view", view);

		glm::mat4 projection;
		projection = glm::perspective(
			// fov
			glm::radians(45.0f), 
			// aspect ratio
			(float)width / (float)height, 
			// near and far clipping planes
			0.1f, 100.0f
		);
		kShader.setMatrix("projection", projection);

		//jShader.draw();
		//pShader.draw();
		kShader.translate(-0.5f, 0.5f);
		kShader.rotate(static_cast<float>(glfwGetTime()));
		kShader.update();
		kShader.draw();

		kShader.translate(0.5f, -0.5f);
		kShader.scale(sin(glfwGetTime()));
		kShader.update();
		kShader.draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	glfwTerminate();
	return 0;
}
