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
	bool moveCamera = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	std::unordered_map<int, int> movementKeys = {
		{GLFW_KEY_W, FORWARD},
		{GLFW_KEY_S, BACKWARD},
		{GLFW_KEY_A, LEFT},
		{GLFW_KEY_D, RIGHT},
		{GLFW_KEY_Z, UP},
		{GLFW_KEY_C, DOWN}
	};
	if (movementKeys.find(key) != movementKeys.end())
	{
		if (moveCamera) world->camera->move(key);
		else world->moveSelected(movementKeys[key]);
	}

	// other actions are one-per-press, so if the key is 
	// being held down end here
	if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_1)
	{
		if (world->doMultiSample)
			glDisable(GL_MULTISAMPLE);
		else
			glEnable(GL_MULTISAMPLE);

		// clear out existing framebuffers and attachments
		world->deleteBuffers();

		// regenerate buffers for toggled multisampling
		world->setupPostProcessing();
		printf("Toggled anti-alias multisampling: %d\n", world->doMultiSample);

	}
	if (key == GLFW_KEY_2)
	{
		world->doBlinnPhong = !world->doBlinnPhong;
		printf("Toggled Blinn-Phong to: %d\n", world->doBlinnPhong);
	}
	if (key == GLFW_KEY_3)
	{
		world->gammaCorrection = !world->gammaCorrection;
		printf("Toggled Gamma Correction %d\n", 
			world->gammaCorrection
		);
	}
	if (key == GLFW_KEY_4)
	{
		//for (auto& object : world->objects)
		//{
		//	object.invertSpec = !object.invertSpec;
		//	std::cout << "Inverted Spec" << std::endl;
		//}

	}
	if (key == GLFW_KEY_5)
	{
		printf("Emission is deprecated");
		//world->object->addEmission = !world->object->addEmission;
		//std::cout << "Toggled Emission" << std::endl;
	}
	if (key == GLFW_KEY_6)
	{
		//world->object->addEmission = true;
		//world->object->animateEmission = !world->object->animateEmission;
		//std::cout << "Toggled Emission Animation" << std::endl;
		printf("Animated emission deprecated");
	}
	if (key == GLFW_KEY_7)
	{
		world->record = !world->record;
		//world->record = true;
		std::cout << "Recording " << world->record << std::endl;
	}
	if (key == GLFW_KEY_8)
	{
		//world->lightingType = (world->lightingType + 1) % world->numLightingTypes;
		//std::cout << "Switched to lighting type " << world->lightingType << std::endl;
		std::cout << "Switching lighting type deprecated, pending multiple lights implementation" << std::endl;
	}

	if (key == GLFW_KEY_EQUAL)
	{
		//world->light->ambient.reset();
		//world->light->diffuse.reset();
		//world->light->specular.reset();
		//world->object->shininess.reset();
		std::cout << "Resetting lights deprecated, pending multiple lights implementation" << std::endl;
	}
	if (key == GLFW_KEY_N)
	{
		for (auto& object : world->objects)
		{
			// only support drawing normals on non-instanced objects for now
			if (object->instances == 0)
			{
				object->drawNormals = !object->drawNormals;
				printf("Toggled %s drawing normals %d\n", 
					object->name.c_str(), object->drawNormals
				);
			}
		}
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
		std::cout << "Attribute change deprecated, pending multiple lights implementation" << std::endl;
		//if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		//{
		//	if (rPressed || gPressed || bPressed)
		//	{
		//		world->light->ambient.update(rChange, gChange, bChange);
		//	}
		//	else
		//	{
		//		world->light->ambient.update(attributeChange);
		//	}

		//	std::string vector = glm::to_string(world->light->ambient.value);
		//	std::cout 
		//		<< "light.ambient=" << vector 
		//		<< " " << rChange << " " << gChange << " " << bChange
		//		<< std::endl;
		//}
		//if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		//{
		//	if (rPressed || gPressed || bPressed)
		//	{
		//		world->light->diffuse.update(rChange, gChange, bChange);
		//	}
		//	else
		//	{
		//		world->light->diffuse.update(attributeChange);
		//	}

		//	std::string vector = glm::to_string(world->light->diffuse.value);
		//	std::cout << "light.diffuse" << "=" << vector << std::endl;
		//}
		//if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		//{
		//	if (rPressed || gPressed || bPressed)
		//	{
		//		world->light->specular.update(rChange, gChange, bChange);
		//	}
		//	else
		//	{
		//		world->light->specular.update(attributeChange);
		//	}

		//	std::string vector = glm::to_string(world->light->specular.value);
		//	std::cout << "light.specular" << "=" << vector << std::endl;
		//}
		//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		//{
		//	world->object->shininess.update(attributeChange);
		//	std::cout << "object.shininess=" << world->object->shininess.value << std::endl;
		//}
	}


	// cycle through materials
	if (key == GLFW_KEY_TAB)
	{
		int direction = FORWARD;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
			direction = BACKWARD;
		world->cycle(direction, SELECTION);
	}
	if (key == GLFW_KEY_F)
	{
		world->camera->canFly = !world->camera->canFly;
		std::cout << "Set fly mode " << world->camera->canFly << std::endl;
	}
	if (key == GLFW_KEY_L)
	{
		// TODO: support for multiple lights
		//int newIndex = (world->light->attenuationIndex + 1) % world->light->numAttenuationSettings;
		//world->light->setAttenuation(newIndex);
		//std::cout << "Set light attenuation to " << world->light->distance << std::endl;
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