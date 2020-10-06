#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void create_window(int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int create_vertex_shader();
int create_fragment_shader();
int create_shader_program(int vert_id, int frag_id);

float map_to_range(float X, float A, float B, float C, float D);

int mouseState = GLFW_RELEASE;
GLFWwindow* window;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(1.0f, 0.69f, 0.4f, 1.0f);\n"
	"}\n\0";

// vertex data
const float vertices[] = {
	  0.5f,  0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	 -0.5f, -0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};


int main()
{
	
	// guess what this does
	create_window(800, 600);

	// build and compile shader program
	int vertexShader = create_vertex_shader();
	int fragmentShader = create_fragment_shader();
	int shaderProgram = create_shader_program(vertexShader, fragmentShader);

	//generate array and buffer objects
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the vertex array
	glBindVertexArray(VAO);

	// bind and set vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// configure vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// bind and set element buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(
			GL_TRIANGLES,
			sizeof(indices) / sizeof(indices[0]),
			GL_UNSIGNED_INT,
			0
		);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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


int create_vertex_shader()
{
	int vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &vertexShaderSource, NULL);
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


int create_fragment_shader()
{
	int fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &fragmentShaderSource, NULL);
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


int create_shader_program(int vert_id, int frag_id)
{
	int prog_id = glCreateProgram();
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
