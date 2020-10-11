#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

void create_window(int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void create_vertex_buffers(
	const float vertices[], int vSize, int vLen, int numAttributes,
	unsigned int* VAO, unsigned int* VBO,
	unsigned int* EBO = NULL, 
	const float indices[] = NULL, int iSize = -1);
static std::string read_shader_file(const char* shader_file);
int create_vertex_shader(const char* path);
int create_fragment_shader(const char* path);
int create_shader_program(const char* vert_path, const char* frag_path);

float map_to_range(float X, float A, float B, float C, float D);

int mouseState = GLFW_RELEASE;
GLFWwindow* window;

// vertex data
const float jamal[] = {
    // position             // colours
	-1.0f , 0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
	-0.5f, 1.0f, 0.0f,      0.0f,  1.0f,  0.0f,
	 0.0f , 0.0f, 0.0f,     0.0f,  0.0f,  1.0f
};
const float persephone[] = {
	 0.0f , 0.0f, 0.0f,
	 0.5f, -1.0f, 0.0f,
	 1.0f , 0.0f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};


int main()
{
	
	// guess what this does
	create_window(800, 600);

	// build and compile shader programs for each array
	int jShader = create_shader_program("colourShape.vs", "colourShape.fs");
	int pShader = create_shader_program("default.vs", "fancy.fs");

	//generate array and buffer objects
	unsigned int pVBO, pVAO;
	unsigned int jVBO, jVAO ;
	create_vertex_buffers(jamal, sizeof(jamal), 6, 1, &jVAO, &jVBO);
	create_vertex_buffers(persephone, sizeof(persephone), 3, 0, &pVAO, &pVBO);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Max number vertex attributes supported: " << nrAttributes << std::endl;


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

		glUseProgram(jShader);
		glBindVertexArray(jVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(pShader);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(pShader, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		
		glBindVertexArray(pVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &jVAO);
	glDeleteBuffers(1, &jVBO);
	//glDeleteBuffers(1, &EBO);

	glDeleteProgram(jShader);
	glDeleteProgram(pShader);

	// clean up
	glfwTerminate();
	return 0;
}


void create_window(int width, int height) 
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

	printf("hello there, mr window\n");
}


float map_to_range(float X, float A, float B, float C, float D)
{
	return (X - A) * ((D - C) / (B - A)) + C;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	int newMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	double x, y;
	if (newMouseState == GLFW_PRESS && mouseState == GLFW_RELEASE)
	{
		mouseState = GLFW_PRESS;
		glfwGetCursorPos(window, &x, &y);
		std::cout 
			<< "X:" << 
			// TODO: make this dynamic to window size (e.g. if it resizes)
			map_to_range((float) x, 0.0f, 800.0f, -1.0f, 1.0f) 
			<< ", Y:" << 
			map_to_range((float) y, 0.0f, 600.0f, 1.0f, -1.0f)
			<< std::endl;
	}
	else if (mouseState == GLFW_PRESS && newMouseState == GLFW_RELEASE)
	{
		mouseState = GLFW_RELEASE;
	}
}


void create_vertex_buffers(
	const float vertices[], int vSize, int vLen, int numAttributes,
	unsigned int* VAO, unsigned int* VBO, 
	unsigned int* EBO, const float indices[], int iSize)
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);
	
	if (EBO != NULL)
	{
		glGenBuffers(1, EBO);
	}

	// bind the vertex array
	glBindVertexArray(*VAO);

	// bind and set vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

	// bind and set element buffers
	if (EBO != NULL && iSize != -1 && EBO != NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
	}

	// generate vertex attributes per attribute in source data
	for (int attr = 0; attr <= numAttributes; attr++)
	{
		int offset = 3 * attr * sizeof(float);
		std::cout << attr << " " << offset << std::endl;		
		glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, vLen * sizeof(float), (void*) offset);
		glEnableVertexAttribArray(attr);
	}

	// unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (EBO != NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


static std::string read_shader_file(const char* shader_file)
{
	std::ifstream file(shader_file);
	if (!file) return std::string();

	file.clear();
	file.seekg(0, std::ios_base::beg);

	std::stringstream sstr;
	sstr << file.rdbuf();
	file.close();

	return sstr.str();
}


int create_vertex_shader(const char* path)
{
	// read fragment shader source from file at path
	std::string source = read_shader_file(path);
	const char* shader_source = source.c_str();
	
	int vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &shader_source, NULL);
	glCompileShader(vid);

	int success;
	char infoLog[512];
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return vid;
}


int create_fragment_shader(const char* path)
{
	// read fragment shader source from file at path
	std::string source = read_shader_file(path);
	const char* shader_source = source.c_str();
	
	int fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &shader_source, NULL);
	glCompileShader(fid);

	int success;
	char infoLog[512];
	glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return fid;
}


int create_shader_program(const char* vert_path, const char* frag_path)
{
	int prog_id = glCreateProgram();
	int vert_id = create_vertex_shader(vert_path);
	int frag_id = create_fragment_shader(frag_path);

	glAttachShader(prog_id, vert_id);
	glAttachShader(prog_id, frag_id);
	glLinkProgram(prog_id);

	int success;
	char infoLog[512];
	glGetProgramiv(prog_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// tidy up by deleting vert and prog shaders
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	return prog_id;
}
