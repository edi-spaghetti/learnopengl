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
	if (key == GLFW_KEY_4)
	{
		world->object->invertSpec = !world->object->invertSpec;
		std::cout << "Inverted Spec" << std::endl;
	}
	if (key == GLFW_KEY_5)
	{
		world->object->addEmission = !world->object->addEmission;
		std::cout << "Toggled Emission" << std::endl;
	}
	if (key == GLFW_KEY_6)
	{
		world->object->addEmission = true;
		world->object->animateEmission = !world->object->animateEmission;
		std::cout << "Toggled Emission Animation" << std::endl;
	}

	if (key == GLFW_KEY_EQUAL)
	{
		world->light->ambient.reset();
		world->light->diffuse.reset();
		world->light->specular.reset();
		world->object->shininess.reset();
	}

	int attributeChange = 0;
	if (key == GLFW_KEY_PAGE_UP) attributeChange = ATTR_INCREASE;
	if (key == GLFW_KEY_PAGE_DOWN) attributeChange = ATTR_DECREASE;

	// detect key state for RGB and check against attribute change to 
	// see if it should go up or down.
	// TODO: according to this code we should be able to modify twp colours at once
	//       e.g. get purple by holding down R and B. However, due to physical limitations
	//       of the keyboard, only two character keys can be held down at once :(
	int rChange = 0, gChange = 0, bChange = 0;
	bool rPressed = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
	if (rPressed) rChange = attributeChange;

	bool gPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
	if (gPressed) gChange = attributeChange;

	bool bPressed = glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
	if (bPressed) bChange = attributeChange;

	// set the values based on colour, attribute and modifier
	if (attributeChange)
	{

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{
			if (rPressed || gPressed || bPressed)
			{
				world->light->ambient.update(rChange, gChange, bChange);
			}
			else
			{
				world->light->ambient.update(attributeChange);
			}

			std::string vector = glm::to_string(world->light->ambient.value);
			std::cout 
				<< "light.ambient=" << vector 
				<< " " << rChange << " " << gChange << " " << bChange
				<< std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			if (rPressed || gPressed || bPressed)
			{
				world->light->diffuse.update(rChange, gChange, bChange);
			}
			else
			{
				world->light->diffuse.update(attributeChange);
			}

			std::string vector = glm::to_string(world->light->diffuse.value);
			std::cout << "light.diffuse" << "=" << vector << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			if (rPressed || gPressed || bPressed)
			{
				world->light->specular.update(rChange, gChange, bChange);
			}
			else
			{
				world->light->specular.update(attributeChange);
			}

			std::string vector = glm::to_string(world->light->specular.value);
			std::cout << "light.specular" << "=" << vector << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			world->object->shininess.update(attributeChange);
			std::cout << "object.shininess=" << world->object->shininess.value << std::endl;
		}
	}


	// cycle through materials
	if (key == GLFW_KEY_TAB)
	{
		bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		world->cycleMaterial(shiftPressed);
	}


	// Deprecated
	if (key == GLFW_KEY_UP)
	{
		world->object->currentZoom += 0.05;
		world->object->setFloat("zoom", world->object->currentZoom);
		std::cout << "set zoom to " << world->object->currentZoom << std::endl;
	}
	else if (key == GLFW_KEY_DOWN)
	{
		world->object->currentZoom -= 0.05;
		world->object->setFloat("zoom", world->object->currentZoom);
		std::cout << "set zoom to " << world->object->currentZoom << std::endl;
	}
	else if (key == GLFW_KEY_LEFT)
	{
		world->object->increaseTransparency();
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		world->object->decreaseTransparency();
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