#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

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
	GLFWwindow* window = createWindow(800, 600);

	// construct shaders and load geometry and textures
	//Shader jShader = Shader("colourShape.vs", "colourShape.fs", jamal);
	//Shader pShader = Shader("default.vs", "default.fs", persephone);
	int nTextures = 2;
	Texture textures[] = { 
		Texture("container.jpg", false, false, GL_CLAMP_TO_EDGE, GL_NEAREST), 
		Texture("awesomeface.png", true, true) 
	};

	Shader kShader = Shader("posColTex.vs", "posColTex.fs", karen, textures, nTextures);
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
		processInput(window);

		// render
		// set the colour to pleasant green
		glClearColor(0.0f, 0.6f, 0.029f, 1.0f);
		// fill er up
		glClear(GL_COLOR_BUFFER_BIT);

		location += velocity;
		//jShader.setFloat("vLocation", location);

		//jShader.draw();
		//pShader.draw();
		kShader.draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	glfwTerminate();
	return 0;
}
