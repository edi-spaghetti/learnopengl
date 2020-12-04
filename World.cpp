#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "World.h"


World::World(GLFWwindow* win, Camera* cam, std::vector<Shader*> objs, 
	MaterialManager* mng, std::vector<LightSource*> lgts)
{

	// TODO: multiple objects + layers
	//       this will allow rendering geo to signify light source's position
	//       without affecting how light hits the other objects / itself

	window = win;
	camera = cam;
	objects = objs;
	matManager = mng;
	lights = lgts;

	camera->world = this;

	std::cout << " // Setup" << std::endl;
	std::cout << "---------------------------------------------" << std::endl;

	// set up framebuffer and render buffer for post processing
	setupPostProcessing();

	// initialise object material uniforms
	for (auto &object : objects)
	{
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
	}

	// initalise light properties on objects
	std::cout << "Initial Setting " << lights.size() << " lights properties" 
		<< std::endl;

	int i = -1;
	for (auto &light : lights)
	{
		i++;

		for (auto& object : objects)
		{
			// type
			object->setInt(light->uniform(i, "type"), light->type);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "type"))
				<< " > " << light->type << std::endl;

			// position
			object->setVec3(light->uniform(i, "position"), light->position);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "position"))
				<< " > " << glm::to_string(light->position) << std::endl;

			// direction
			object->setVec3(light->uniform(i, "direction"), light->direction);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "direction"))
				<< " > " << glm::to_string(light->direction) << std::endl;

			// ambient
			object->setVec3(light->uniform(i, "ambient"), glm::vec3(light->ambient.value));
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "ambient"))
				<< " > " << glm::to_string(light->ambient.value) << std::endl;

			// diffuse
			object->setVec3(light->uniform(i, "diffuse"), glm::vec3(light->diffuse.value));
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "diffuse"))
				<< " > " << glm::to_string(light->diffuse.value) << std::endl;

			// specular
			object->setVec3(light->uniform(i, "specular"), glm::vec3(light->specular.value));
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "specular"))
				<< " > " << glm::to_string(light->specular.value) << std::endl;

			// constant
			object->setFloat(light->uniform(i, "constant"), light->constant);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "constant"))
				<< " > " << light->constant << std::endl;

			// linear
			object->setFloat(light->uniform(i, "linear"), light->linear);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "linear"))
				<< " > " << light->linear << std::endl;

			// quadratic
			object->setFloat(light->uniform(i, "quadratic"), light->quadratic);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "quadratic"))
				<< " > " << light->quadratic << std::endl;

			// innerBeam
			object->setFloat(light->uniform(i, "innerBeam"), light->innerBeam);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "innerBeam"))
				<< " > " << light->innerBeam << std::endl;

			// outerBeam
			object->setFloat(light->uniform(i, "outerBeam"), light->outerBeam);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "outerBeam"))
				<< " > " << light->outerBeam << std::endl;
		}

		// outline colour
		light->setVec3("outlineColour", light->outlineColour);
		if (this->doLogging) std::cout << "light " << i
			<< "set outline colour to "
			<< glm::to_string(light->outlineColour)
			<< std::endl;
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
void World::setupPostProcessing()
{
	// make sure screen width and height have been set up before 
	// we write out frame buffers
	processInput(window, &screenWidth, &screenHeight);
	
	// set up frame buffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// determine if we're doing multisampling
	GLboolean doMultiSample;
	glGetBooleanv(GL_MULTISAMPLE, &doMultiSample);
	GLenum target;
	if (doMultiSample) target = GL_TEXTURE_2D_MULTISAMPLE;
	else target = GL_TEXTURE_2D;

	// generate textures
	glGenTextures(1, &tcb);
	glBindTexture(target, tcb);
	if (doMultiSample)
		glTexImage2DMultisample(
			GL_TEXTURE_2D_MULTISAMPLE, SAMPLE_COUNT, GL_RGB, 
			screenWidth, screenHeight, 
			GL_TRUE);
	else
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB, 
			screenWidth, screenHeight, 
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL
		);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach texture to frame buffer
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, tcb, 0
	);

	// set up render buffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if (doMultiSample)
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER, SAMPLE_COUNT, GL_DEPTH24_STENCIL8,
			screenWidth, screenHeight
		);
	else
		glRenderbufferStorage(
			GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight
		);

	// attach renderbuffer to depth and stencil attachment of framebuffer
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo
	);

	// check it completed
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	std::map<int, std::string> result = {
		{GL_FRAMEBUFFER_COMPLETE, "complete"},
		{GL_FRAMEBUFFER_UNDEFINED, "undefined"},
		{GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "incomplete attachment"},
		{GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "missing image attachment"},
		{GL_FRAMEBUFFER_UNSUPPORTED, "unsupported"},
		{GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "incomplete multi-sample"},
		{GL_INVALID_ENUM, "invalid enum"}
	};
	std::cout << "Framebuffer status: " << result[status] << std::endl;
	
	
	// and again for reverse
	// TODO: some kind of management class for multiple frame buffers
	// -----------------------------------------------------------------------
	glGenFramebuffers(1, &fboReverse);
	glBindFramebuffer(GL_FRAMEBUFFER, fboReverse);

	// generate textures
	glGenTextures(1, &tcbReverse);
	glBindTexture(GL_TEXTURE_2D, tcbReverse);
	if (doMultiSample)
		glTexImage2DMultisample(
			GL_TEXTURE_2D_MULTISAMPLE, SAMPLE_COUNT, GL_RGB,
			screenWidth, screenHeight,
			GL_TRUE);
	else
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB,
			screenWidth, screenHeight,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL
		);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach texture to frame buffer
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, tcbReverse, 0
	);

	// set up render buffer
	glGenRenderbuffers(1, &rboReverse);
	glBindRenderbuffer(GL_RENDERBUFFER, rboReverse);
	if (doMultiSample)
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER, SAMPLE_COUNT, GL_DEPTH24_STENCIL8,
			screenWidth, screenHeight
		);
	else
		glRenderbufferStorage(
			GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight
		);

	// attach renderbuffer to depth and stencil attachment of framebuffer
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboReverse
	);

	// check it completed
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	std::cout << "Framebuffer Reverse status: " << result[status] << std::endl;
	
	
	// Create Intermediate Buffer if Multi-Sampling
	// -----------------------------------------------------------------------
	glGenFramebuffers(1, &iFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, iFBO);

	// generate textures
	glGenTextures(1, &iTCB);
	glBindTexture(GL_TEXTURE_2D, iTCB);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		screenWidth, screenHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach texture to frame buffer
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iTCB, 0
	);

	// set up render buffer
	glGenRenderbuffers(1, &iRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, iRBO);
	glRenderbufferStorage(
		GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight
	);

	// attach renderbuffer to depth and stencil attachment of framebuffer
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, iRBO
	);

	// check it completed
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	std::cout << "Framebuffer Intermediary status: " << result[status] << std::endl;
	
	// clean up
	// --------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


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

	// draw first pass on custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	// set the colour to grey-ish
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	drawObjects();

	// set the reverse view on each object
	skybox.setMatrix("view", glm::mat4(glm::mat3(reverseView)));
	for (auto& object : objects)
	{
		object->setMatrix("view", reverseView);
		if (object->drawNormals) object->setMatrix("view", reverseView, true);
	}
	for (auto& light : lights)
	{
		light->setMatrix("view", reverseView);
	}

	// re-render to second buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboReverse);
	glEnable(GL_DEPTH_TEST);
	// set the colour to green-ish black-ish
	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	drawObjects();

	// set up default frame buffer with appropriate settings
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// temporarily disable everything
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// now draw each layer on top of the previous
	// blit fbo into intermediary (that can be read) and draw to quad
	glBindFramebuffer(GL_FRAMEBUFFER, iFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, iFBO);
	glBlitFramebuffer(
		0, 0, screenWidth, screenHeight,
		0, 0, screenWidth, screenHeight,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	screen.screenDraw(iTCB);

	// now blit reverse into intermediary and draw to quad
	glBindFramebuffer(GL_FRAMEBUFFER, iFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboReverse);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, iFBO);
	glBlitFramebuffer(
		0, 0, screenWidth, screenHeight,
		0, 0, screenWidth, screenHeight,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mirror.screenDraw(iTCB);

	// stop logging after first frame
	if (this->doLogging) this->doLogging = false;
}


void World::drawObjects()
{
	// disable writing to stencil before doing outline pass
	glStencilMask(0x00);

	for (auto& object : objects)
	{
		object->useTextureUnit(GL_TEXTURE_CUBE_MAP, object->numTextures, skybox.ID);
		object->draw();
	}
	int i = -1;
	for (auto& light : lights)
	{
		i++;
		if (doLogging) std::cout << "Light " << i << std::endl;
		if (light->type != SPOTLIGHT) //&& i != currentSelection)
			light->draw();
	}

	if (doLogging) printf("Drawing skybox\n");
	skybox.draw();

	// transparency pass
	if (this->doLogging) std::cout << "Rendering opacity pass" << std::endl;
	for (auto& object : objects)
	{
		if (object->modelLoaded)
		{
			// sort meshes by distance
			std::map<float, Mesh> meshByDistance;
			for (Mesh mesh : object->mod.getTransparentMeshes())
			{
				glm::vec3 position = object->position + mesh.localPosition;
				float distance = glm::length(camera->position - position);
				meshByDistance[distance] = mesh;
			}

			// now render in reverse order
			for (std::map<float, Mesh>::reverse_iterator it = meshByDistance.rbegin();
				it != meshByDistance.rend();
				++it)
			{
				it->second.draw(object->ID, object->instances);
				if (object->drawNormals)
				{
					if (this->doLogging) std::cout << "Rendering normals" << std::endl;
					it->second.draw(object->normID, object->instances);
				}
			}
		}
	}
	// outline pass
	lights[currentSelection]->drawWithOutline();
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
	skybox.setMatrix("view", glm::mat4(glm::mat3(view)));
	skybox.setMatrix("projection", projection);
	for (auto& object : objects)
	{
		if (!object->instances) object->setMatrix("model", object->model);
		object->setMatrix("view", view);
		object->setMatrix("projection", projection);

		if (object->drawNormals)
		{
			if (!object->instances) object->setMatrix("model", object->model, true);
			object->setMatrix("view", view, true);
			object->setMatrix("projection", projection, true);
		}
	}
	for (auto &light : lights)
	{
		light->setMatrix("model", light->model);
		light->setMatrix("view", view);
		light->setMatrix("projection", projection);
	}

	// settings
	// TODO: list of named attributes so we can set dynamically

	// TODO: refactor vertex shader to support multiple lights
	// gouraud / phong
	//object->setInt("toggleGouraudPhong", int(toggleGouraudPhong));
	for (auto& object : objects)
	{
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
		object->setInt("numLights", this->lights.size());
		if (this->doLogging) std::cout << "numLights > " << this->lights.size() << std::endl;
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
	}

	// update light uniforms
	int i = -1;
	for (auto &light : lights)
	{
		i++;

		if (this->doLogging) std::cout << "Light: " << i << " type: " << light->type 
			<< std::endl;

		// not doing lighting cube in view space, so always set in world space
		glm::mat4 normalMatrix = light->getNormalMatrix();
		light->setMatrix("normalMatrix", normalMatrix);
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
		light->setVec3("light.position", light->position);
		if (this->doLogging) std::cout << i << " light.position > "
			<< glm::to_string(light->position) << std::endl;

		// phong materials

		// ambient
		light->setVec3("light.ambient", glm::vec3(light->ambient.value));
		if (this->doLogging) std::cout << i << " light.ambient > "
			<< glm::to_string(glm::vec3(light->ambient.value)) << std::endl;

		// diffuse
		light->setVec3("light.diffuse", glm::vec3(light->diffuse.value));
		if (this->doLogging) std::cout << i << " light.diffuse > "
			<< glm::to_string(glm::vec3(light->diffuse.value)) << std::endl;

		// specular
		light->setVec3("light.specular", glm::vec3(light->specular.value));
		if (this->doLogging) std::cout << i << " light.specular > "
			<< glm::to_string(glm::vec3(light->specular.value)) << std::endl;

		// shininess
		light->setFloat("light.shininess", light->shininess.value);
		if (this->doLogging) std::cout << i << " light.shininess > "
			<< light->shininess.value << std::endl;

		for (auto& object : objects)
		{

			// position
			object->setVec3(light->uniform(i, "position"), light->position);
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "position"))
				<< " > " << glm::to_string(light->position) << std::endl;

			// direction
			object->setVec3(light->uniform(i, "direction"), light->direction);
			if (this->doLogging) std::cout << "object "
				<< (light->uniform(i, "direction")) << " > "
				<< glm::to_string(light->direction) << std::endl;

			if (light->type == SPOTLIGHT)
			{

				// inner beam
				object->setFloat(light->uniform(i, "innerBeam"), light->innerBeam);
				if (this->doLogging) std::cout << "object "
					<< (light->uniform(i, "innerBeam")) << " > "
					<< light->innerBeam << std::endl;

				// outer beam
				object->setFloat(light->uniform(i, "outerBeam"), light->outerBeam);
				if (this->doLogging) std::cout << "object "
					<< (light->uniform(i, "outerBeam")) << " > "
					<< light->outerBeam << std::endl;
			}
			if (light->type != DIRECTIONAL)
			{

				// attenuation
				// -----------

				// constant
				object->setFloat(light->uniform(i, "constant"), light->constant);
				if (this->doLogging) std::cout << "object "
					<< (light->uniform(i, "constant")) << " > "
					<< light->constant << std::endl;

				// linear
				object->setFloat(light->uniform(i, "linear"), light->linear);
				if (this->doLogging) std::cout << "object "
					<< (light->uniform(i, "linear")) << " > "
					<< light->linear << std::endl;

				// quadratic
				object->setFloat(light->uniform(i, "quadratic"), light->quadratic);
				if (this->doLogging) std::cout << "object "
					<< (light->uniform(i, "quadratic")) << " > "
					<< light->quadratic << std::endl;
			}

			// phong materials

			// ambient
			object->setVec3(light->uniform(i, "ambient"), glm::vec3(light->ambient.value));
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "ambient"))
				<< " > " << glm::to_string(glm::vec3(light->ambient.value)) << std::endl;

			// diffuse
			object->setVec3(light->uniform(i, "diffuse"), glm::vec3(light->diffuse.value));
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "diffuse"))
				<< " > " << glm::to_string(glm::vec3(light->diffuse.value)) << std::endl;

			// specular
			object->setVec3(light->uniform(i, "specular"), glm::vec3(light->specular.value));
			if (this->doLogging) std::cout << "object " << (light->uniform(i, "specular"))
				<< " > " << glm::to_string(glm::vec3(light->specular.value)) << std::endl;

		}
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

	reverseView = glm::lookAt(
		camera->position,
		camera->position - camera->front,
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
	for (auto &light : lights)
	{
		if (light->type == SPOTLIGHT)
		{
			light->position = camera->position;
			light->direction = camera->front;
		}
		else
		{
			// do position > velocity > acceleration type movement calculation
		}

		glm::mat4 model = light->genModelMatrix();
		light->model = model;
	}

	// now do the objects
	for (auto& object : objects)
	{
		glm::mat4 model = object->genModelMatrix();
		object->model = model;
	}
}


float World::getLightRotationAngle()
{
	// TODO: implement for multiple lights
	if (staticLight) return glm::radians(currentLightRotationAngle);

	currentLightRotationAngle += 0.05f;
	return glm::radians(currentLightRotationAngle);
}


void World::cycle(const int direction, const int attribute)
{
	if (attribute == SELECTION)
	{
		int delta = 1;
		if (direction == BACKWARD) delta = -1;

		this->currentSelection += delta;
		this->currentSelection %= lights.size();

		std::cout << "Cycled selection to light " << currentSelection << std::endl;
	}
	else if (attribute == MATERIAL)
	{
		// TODO
		//if (direction == FORWARD)
		//{
		//	matManager->currentMaterial = (matManager->numMaterials + 1) % matManager->numMaterials;
		//}
		//else
		//{
		//	if (matManager->currentMaterial)
		//	{
		//		matManager->currentMaterial -= 1;
		//	}
		//	else
		//	{
		//		matManager->currentMaterial = matManager->numMaterials - 1;
		//	}
		//}

			//light->loadMaterials(*(matManager->materials[matManager->currentMaterial]));
		//std::cout
		//	<< "Material not set to "
		//	<< matManager->currentMaterial
		//	<< " - method deprecated pending multiple lights implementation"
		//	<< std::endl;
	}
}


void World::moveSelected(const int direction)
{
	lights[currentSelection]->move(direction, deltaTime);
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