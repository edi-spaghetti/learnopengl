#include "World.h"
#include "demo/space.h"


int main()
{
	printf("Starting...\n");

	// guess what this does
	int width = 800;
	int height = 600;
	GLFWwindow* window = createWindow(width, height);

	World* world = spaceGame(window);

	// start render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		world->update();

		// render
		world->draw();

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	world->tearDown();
	delete world;

	glfwTerminate();
	return 0;
}
