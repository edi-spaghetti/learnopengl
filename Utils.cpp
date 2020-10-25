#include "Utils.h"

// globals
int mouseState = GLFW_RELEASE;


GLFWwindow* createWindow(int width, int height)
{
	// instantiate a GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

		// create a window object and set context
		GLFWwindow* window;
		window = glfwCreateWindow(width, height, "My Beautiful Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	// register window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialise GLAD, so we're ready to call OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		exit(-2);
	}

	return window;
}


float map_to_range(float X, float A, float B, float C, float D)
{
	return (X - A) * ((D - C) / (B - A)) + C;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void setupUserControls(GLFWwindow* window, World* world)
{
	glfwSetWindowUserPointer(window, world);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}



void processInput(GLFWwindow* window, int* width, int* height)
{

	// upate window dimension variables if changed
	glfwGetWindowSize(window, width, height);
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// refactor this to mouse button callback
	int newMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	double x, y;
	if (newMouseState == GLFW_PRESS && mouseState == GLFW_RELEASE)
	{
		mouseState = GLFW_PRESS;
		glfwGetCursorPos(window, &x, &y);
		std::cout
			<< "X:" <<
			// TODO: make this dynamic to window size (e.g. if it resizes)
			map_to_range(x, 0.0f, *width, -1.0f, 1.0f)
			<< ", Y:" <<
			map_to_range(y, 0.0f, *height, 1.0f, -1.0f)
			<< std::endl;
	}
	else if (mouseState == GLFW_PRESS && newMouseState == GLFW_RELEASE)
	{
		mouseState = GLFW_RELEASE;
	}
}


void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	World* world = (World*) glfwGetWindowUserPointer(window);

	// process camera movement first so we can have continuous movement
	if (key == GLFW_KEY_W ||
		key == GLFW_KEY_S ||
		key == GLFW_KEY_A ||
		key == GLFW_KEY_D)
	{
		world->camera->move(key);
	}

	// other actions are one-per-press, so if the key is 
	// being held down end here
	if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_1)
	{
		world->staticLight = !world->staticLight;
	}
	if (key == GLFW_KEY_2)
	{
		world->shadeInViewSpace = !world->shadeInViewSpace;
		std::cout 
			<< "Toggled shading in viewspace " << world->shadeInViewSpace 
		<< std::endl;
	}
	if (key == GLFW_KEY_3)
	{
		world->toggleGouraudPhong = !world->toggleGouraudPhong;
		std::cout
			<< "Toggled Gouraud/Phong shading " << world->toggleGouraudPhong
			<< std::endl;
	}

	// TODO: make this less repetitive
	bool shaderAttributeChange;
	bool checkAttrbuteKeys = false;
	if (key == GLFW_KEY_PAGE_UP)
	{
		checkAttrbuteKeys = true;
		shaderAttributeChange = true;
	}
	if (key == GLFW_KEY_PAGE_DOWN)
	{
		checkAttrbuteKeys = true;
		shaderAttributeChange = false;
	}

	if (checkAttrbuteKeys)
	{
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			world->shader->ambient.update(shaderAttributeChange);
			std::cout << "ambient=" << world->shader->ambient.value << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			world->shader->specular.update(shaderAttributeChange);
			std::cout << "specular=" << world->shader->specular.value << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			world->shader->shininess.update(shaderAttributeChange);
			std::cout << "shininess=" << world->shader->shininess.value << std::endl;
		}
	}


	if (key == GLFW_KEY_UP)
	{
		world->shader->currentZoom += 0.05;
		world->shader->setFloat("zoom", world->shader->currentZoom);
		std::cout << "set zoom to " << world->shader->currentZoom << std::endl;
	}
	else if (key == GLFW_KEY_DOWN)
	{
		world->shader->currentZoom -= 0.05;
		world->shader->setFloat("zoom", world->shader->currentZoom);
		std::cout << "set zoom to " << world->shader->currentZoom << std::endl;
	}
	else if (key == GLFW_KEY_LEFT)
	{
		world->shader->increaseTransparency();
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		world->shader->decreaseTransparency();
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	World* world = (World*)glfwGetWindowUserPointer(window);

	world->camera->updateDirection(xpos, ypos);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	World* world = (World*)glfwGetWindowUserPointer(window);

	world->camera->updateFOV(static_cast<float>(yoffset));
}