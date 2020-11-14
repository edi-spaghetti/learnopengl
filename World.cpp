#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "World.h"


World::World(GLFWwindow* win, Camera* cam, Shader* obj, 
	MaterialManager* mng, unsigned int nLgts, LightSource* lgts)
{

	// TODO: multiple objects + layers
	//       this will allow rendering geo to signify light source's position
	//       without affecting how light hits the other objects / itself

	window = win;
	camera = cam;
	object = obj;
	matManager = mng;

	numLights = nLgts;
	for (int i = 0; i < nLgts; i++)
	{
		lights[i] = &lgts[i];
	}

	camera->world = this;

	std::cout << " // Setup" << std::endl;
	std::cout << "---------------------------------------------" << std::endl;

	// initialise object material uniforms
	if (!object->texLoaded)
	{
		// ambient
		object->setVec3("material.ambient", object->ambient.value);
		if (this->doLogging) std::cout << "object material.ambient > " 
			<< glm::to_string(object->ambient.value) << std::endl;

		// diffuse
		object->setVec3("material.diffuse", object->diffuse.value);
		if (this->doLogging) std::cout << "object material.diffuse > " 
			<< glm::to_string(object->diffuse.value) << std::endl;

		// specular
		object->setVec3("material.specular", object->specular.value);
		if (this->doLogging) std::cout << "object material.specular > " 
			<< glm::to_string(object->specular.value) << std::endl;
	}

	// shininess
	object->setFloat("material.shininess", object->shininess.value);
	if (this->doLogging) std::cout << "object material.shininess > " 
		<< object->shininess.value << std::endl;

	// initalise light properties on objects
	std::cout << "Initial Setting " + lights[0]->uniform(numLights, "properties") 
		<< std::endl;

	for (int i = 0; i < nLgts; i++)
	{
		// type
		object->setInt(lights[i]->uniform(i, "type"), lights[i]->type);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "type")) 
			<< " > " << lights[i]->type << std::endl;

		// position
		object->setVec3(lights[i]->uniform(i, "position"), lights[i]->position);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "position"))
			<< " > " << glm::to_string(lights[i]->position) << std::endl;

		// direction
		object->setVec3(lights[i]->uniform(i, "direction"), lights[i]->direction);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "direction"))
			<< " > " << glm::to_string(lights[i]->direction) << std::endl;

		// ambient
		object->setVec3(lights[i]->uniform(i, "ambient"), glm::vec3(lights[i]->ambient.value));
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "ambient")) 
			<< " > " << glm::to_string(lights[i]->ambient.value) << std::endl;
		
		// diffuse
		object->setVec3(lights[i]->uniform(i, "diffuse"), glm::vec3(lights[i]->diffuse.value));
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "diffuse")) 
			<< " > " << glm::to_string(lights[i]->diffuse.value) << std::endl;
		
		// specular
		object->setVec3(lights[i]->uniform(i, "specular"), glm::vec3(lights[i]->specular.value));
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "specular")) 
			<< " > " << glm::to_string(lights[i]->specular.value) << std::endl;

		// constant
		object->setFloat(lights[i]->uniform(i, "constant"), lights[i]->constant);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "constant"))
			<< " > " << lights[i]->constant << std::endl;

		// linear
		object->setFloat(lights[i]->uniform(i, "linear"), lights[i]->linear);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "linear"))
			<< " > " << lights[i]->linear << std::endl;

		// quadratic
		object->setFloat(lights[i]->uniform(i, "quadratic"), lights[i]->quadratic);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "quadratic"))
			<< " > " << lights[i]->quadratic << std::endl;

		// innerBeam
		object->setFloat(lights[i]->uniform(i, "innerBeam"), lights[i]->innerBeam);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "innerBeam"))
			<< " > " << lights[i]->innerBeam << std::endl;

		// outerBeam
		object->setFloat(lights[i]->uniform(i, "outerBeam"), lights[i]->outerBeam);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "outerBeam"))
			<< " > " << lights[i]->outerBeam << std::endl;

	}

	// TODO: re-implement for multiple lights
	//if (light->materialLoaded)
	//{
	//	for (int i = 0; i < matManager->numMaterials; i++)
	//	{
	//		if (matManager->materials[i] == &(light->material))
	//		{
	//			matManager->currentMaterial = i;
	//			break;
	//		}
	//	}
	//}

}

World::~World()
{

}

// main functions to be called mainly from main
// ---------------------------------------------------------------------------
void World::update()
{
	if (doLogging) std::cout << " // Update" << std::endl;
	if (doLogging) std::cout << "--------------------------------------------" << std::endl;

	World::updateTime();
	World::updateScreen();
	World::updateView();
	World::updateProjection();
	World::updateModel();
	World::updateAttributes();
	World::exportCurrentFrame();
}


void World::draw()
{
	if (doLogging) std::cout << " // Draw" << std::endl;
	if (doLogging) std::cout << "--------------------------------------------" << std::endl;

	object->draw();
	for (int i = 0; i < numLights; i++)
	{
		if (lights[i]->type != SPOTLIGHT) lights[i]->draw();
	}

	// stop logging after first frame
	if (this->doLogging) this->doLogging = false;
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
	// set MVP matrices
	object->setMatrix("model", object->model);
	object->setMatrix("view", view);
	object->setMatrix("projection", projection);
	for (int i = 0; i < numLights; i++)
	{
		lights[i]->setMatrix("model", lights[i]->model);
		lights[i]->setMatrix("view", view);
		lights[i]->setMatrix("projection", projection);
	}

	// settings
	// TODO: list of named attributes so we can set dynamically

	// TODO: refactor vertex shader to support multiple lights
	// gouraud / phong
	//object->setInt("toggleGouraudPhong", int(toggleGouraudPhong));
	// invert spec
	object->setBool("invertSpec", object->invertSpec);
	if (this->doLogging) std::cout << "invertSpec > " << object->invertSpec << std::endl;
	// emission
	object->setBool("addEmission", object->addEmission);
	if (this->doLogging) std::cout << "addEmission > " << object->addEmission << std::endl;
	// emission animation
	object->setBool("animateEmission", object->animateEmission);
	if (this->doLogging) std::cout << "animateEmission > "
		<< object->animateEmission << std::endl;
	// time
	object->setFloat("time", static_cast<float>(glfwGetTime()));
	if (this->doLogging) std::cout << "time > "
		<< static_cast<float>(glfwGetTime()) << std::endl;
	// number of lights
	object->setInt("numLights", this->numLights);
	if (this->doLogging) std::cout << "numLights > " << this->numLights << std::endl;
	// view position
	object->setVec3("viewPos", this->camera->position);
	if (this->doLogging) std::cout << "object viewPos > "
		<< glm::to_string(this->camera->position) << std::endl;
	// normal matrix
	object->setMatrix("normalMatrix", object->getNormalMatrix());
	if (this->doLogging) std::cout << "object normalMatrix > " << std::endl;
	
	// TODO: dict (hash table?) of booleans so we can set all at once
	// update material uniforms
	object->setFloat("material.shininess", 32.0f);// object->shininess.value);
	if (this->doLogging) std::cout << "object material.shininess > "
		<< object->shininess.value << std::endl;

	// update light uniforms
	for (int i = 0; i < numLights; i++)
	{
		if (this->doLogging) std::cout << "Light: " << i << " type: " << lights[i]->type 
			<< std::endl;

		// not doing lighting cube in view space, so always set in world space
		glm::mat4 normalMatrix = lights[i]->getNormalMatrix();
		lights[i]->setMatrix("normalMatrix", normalMatrix);
		if (this->doLogging)
		{
			std::cout << i << " light normalMatrix > " << std::endl;
			for (int j = 0; j < 4; j++)
			{
				std::cout  << normalMatrix[j][0] 
					<< " " << normalMatrix[j][1] 
					<< " " << normalMatrix[j][2] 
					<< " " << normalMatrix[j][3] 
				<< std::endl;
			}
		}

		// position
		object->setVec3(lights[i]->uniform(i, "position"), lights[i]->position);
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "position"))
			<< " > " << glm::to_string(lights[i]->position) << std::endl;
		lights[i]->setVec3("light.position", lights[i]->position);
		if (this->doLogging) std::cout << i << " light.position > " 
			<< glm::to_string(lights[i]->position) << std::endl;

		// direction
		object->setVec3(lights[i]->uniform(i, "direction"), lights[i]->direction);
		if (this->doLogging) std::cout << "object "
			<< (lights[i]->uniform(i, "direction")) << " > "
			<< glm::to_string(lights[i]->direction) << std::endl;

		if (lights[i]->type == SPOTLIGHT)
		{
			
			// inner beam
			object->setFloat(lights[i]->uniform(i, "innerBeam"), lights[i]->innerBeam);
			if (this->doLogging) std::cout << "object " 
				<< (lights[i]->uniform(i, "innerBeam")) << " > " 
				<< lights[i]->innerBeam << std::endl;
			
			// outer beam
			object->setFloat(lights[i]->uniform(i, "outerBeam"), lights[i]->outerBeam);
			if (this->doLogging) std::cout << "object " 
				<< (lights[i]->uniform(i, "outerBeam")) << " > " 
				<< lights[i]->outerBeam << std::endl;
		}
		if (lights[i]->type != DIRECTIONAL)
		{
			
			// attenuation
			// -----------

			// constant
			object->setFloat(lights[i]->uniform(i, "constant"), lights[i]->constant);
			if (this->doLogging) std::cout << "object " 
				<< (lights[i]->uniform(i, "constant")) << " > " 
				<< lights[i]->constant << std::endl;

			// linear
			object->setFloat(lights[i]->uniform(i, "linear"), lights[i]->linear);
			if (this->doLogging) std::cout << "object " 
				<< (lights[i]->uniform(i, "linear")) << " > " 
				<< lights[i]->linear << std::endl;

			// quadratic
			object->setFloat(lights[i]->uniform(i, "quadratic"), lights[i]->quadratic);
			if (this->doLogging) std::cout << "object " 
				<< (lights[i]->uniform(i, "quadratic")) << " > " 
				<< lights[i]->quadratic << std::endl;
		}

		// phong materials
		
		// ambient
		lights[i]->setVec3("light.ambient", glm::vec3(lights[i]->ambient.value));
		if (this->doLogging) std::cout << i << " light.ambient > "
			<< glm::to_string(glm::vec3(lights[i]->ambient.value)) << std::endl;
		object->setVec3(lights[i]->uniform(i, "ambient"), glm::vec3(lights[i]->ambient.value));
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "ambient"))
			<< " > " << glm::to_string(glm::vec3(lights[i]->ambient.value)) << std::endl;

		// diffuse
		lights[i]->setVec3("light.diffuse", glm::vec3(lights[i]->diffuse.value));
		if (this->doLogging) std::cout << i << " light.diffuse > "
			<< glm::to_string(glm::vec3(lights[i]->diffuse.value)) << std::endl;
		object->setVec3(lights[i]->uniform(i, "diffuse"), glm::vec3(lights[i]->diffuse.value));
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "diffuse"))
			<< " > " << glm::to_string(glm::vec3(lights[i]->diffuse.value)) << std::endl;

		// specular
		lights[i]->setVec3("light.specular", glm::vec3(lights[i]->specular.value));
		if (this->doLogging) std::cout << i << " light.specular > "
			<< glm::to_string(glm::vec3(lights[i]->specular.value)) << std::endl;
		object->setVec3(lights[i]->uniform(i, "specular"), glm::vec3(lights[i]->specular.value));
		if (this->doLogging) std::cout << "object " << (lights[i]->uniform(i, "specular"))
			<< " > " << glm::to_string(glm::vec3(lights[i]->specular.value)) << std::endl;

		// shininess
		lights[i]->setFloat("light.shininess", lights[i]->shininess.value);
		if (this->doLogging) std::cout << i << " light.shininess > "
			<< lights[i]->shininess.value << std::endl;
	}
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
}


void World::updateModel()
{
	// update lights first
	for (int i = 0; i < numLights; i++)
	{
		if (lights[i]->type == SPOTLIGHT)
		{
			lights[i]->position = camera->position;
			lights[i]->direction = camera->front;
		}
		else
		{
			// do position > velocity > acceleration type movement calculation
		}


		// reset light's model matrix back to identity
		lights[i]->model = glm::mat4(1.0f);

		// add rotation (if any)
		if (lights[i]->rotating) lights[i]->model = glm::rotate(
			lights[i]->model, lights[i]->rotationAngle, lights[i]->pointOfRotation
		);

		// get new light position now so translation is not applied twice
		// TODO: implement proper movement, not just rotation
		//       including pulling position from camera if it is a spotlight
		//glm::vec3 newLightPos = glm::vec3(lights[i]->model * glm::vec4(lights[i]->initPosition, 1.0f));
		//if (!this->shadeInViewSpace) newLightPos = glm::vec3(this->view * glm::vec4(newLightPos, 1.0f));
		//lights[i]->setPosition(newLightPos);

		lights[i]->model = glm::translate(lights[i]->model, lights[i]->initPosition);
		lights[i]->model = glm::scale(lights[i]->model, lights[i]->size);
	}

	// now do the object
	object->model = glm::mat4(1.0f);

	// TODO: object movement
}


float World::getLightRotationAngle()
{
	// TODO: implement for multiple lights
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

	//light->loadMaterials(*(matManager->materials[matManager->currentMaterial]));
	std::cout 
		<< "Material not set to " 
		<< matManager->currentMaterial 
		<< " - method deprecated pending multiple lights implementation" 
	<< std::endl;
}


void World::exportCurrentFrame()
{
	// check if the red light is on
	if (!record) return;

	// check if we're ready for a new frame
	if (currentFrame - recordingStartedAt < currentOutputFrame / outputFPS) return;

	unsigned char* Buffer;
	Buffer = new unsigned char[screenWidth * screenHeight * 3];
	if (!Buffer) return;

	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, Buffer);

	std::string filename("out\\recording." + std::to_string(currentOutputFrame) + ".png");

	stbi_flip_vertically_on_write(true);
	stbi_write_png(filename.c_str(), screenWidth, screenHeight, 3, Buffer, screenWidth * 3);

	// increment output frame ready for next image capture
	currentOutputFrame++;

	delete Buffer;

}