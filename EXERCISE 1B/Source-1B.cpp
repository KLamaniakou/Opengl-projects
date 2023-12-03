//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1B της OpenGL

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>



// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

/// <summary>
/// //////////////////////////////////////////////
/// </summary>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

////////////////////////////////////////////// <summary>
/// Add camera function here
/// </summary>

void camera_function()
{}

/////////////////////////////////////////////////

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
///////////////////////////////////////////////////



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(950, 950, u8"Εργασία 1Β", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// background
	glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1BVertexShader.vertexshader", "P1BFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(5.0f, 5.0f, 5.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
		glm::vec3(0.0f, 0.0f, 1.0f)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	
	//half values 
	GLfloat len = 5.0f, wid=2.5f, heig=2.5f;


	//Cube data
	static const GLfloat cube[] =
	{
        //back square
		-1.0f, -6.0f, -5.0f,
		1.0f, -6.0f, -5.0f,
		1.0f, -4.0f, -5.0f,

		-1.0f, -6.0f, -5.0f,
		1.0f, -4.0f, -5.0f,
		-1.0f, -4.0f, -5.0f,
		//front square
		-1.0f, -6.0f, -3.0f,
		1.0f, -6.0f, -3.0f,
		1.0f, -4.0f, -3.0f,

		-1.0f, -6.0f, -3.0f,
		1.0f, -4.0f, -3.0f,
		-1.0f, -4.0f, -3.0f,
		//left
		-1.0f, -4.0f, -3.0f,
		-1.0f, -4.0f, -5.0f,
		-1.0f, -6.0f, -3.0f,

		-1.0f, -4.0f, -5.0f,
		-1.0f, -4.0f, -3.0f,
		-1.0f, -6.0f, -3.0f,
		//right
		 1.0f, -4.0f, -3.0f,
		 1.0f, -4.0f, -5.0f,
		 1.0f, -6.0f, -3.0f,

		 1.0f, -4.0f, -5.0f,
		 1.0f, -4.0f, -3.0f,
		 1.0f, -6.0f, -3.0f,
		//up
		1.0f, -6.0f, -5.0f,
		-1.0f, -6.0f, -5.0f,
		-1.0f, -6.0f, -3.0f,

		-1.0f, -6.0f, -3.0f,
		1.0f, -6.0f, -5.0f,
		1.0f, -6.0f, -3.0f,
		//down
		1.0f, -4.0f, -5.0f,
		-1.0f, -4.0f, -5.0f,
		-1.0f, -4.0f, -3.0f,

		-1.0f, -4.0f, -3.0f,
		1.0f, -4.0f, -5.0f,
		1.0f, -4.0f, -3.0f,
	};
	//color for the cube
	GLfloat c = 1.0f;
	static const GLfloat color[] = {
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,
		1.0f,  1.0f,  1.0f,c,

	};

	//Shape A data
	static const GLfloat A[] =
	{
		-5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 0.0f,
		5.0f, 2.5f, 0.0f,

		-5.0f, -2.5f, 0.0f,
		5.0f, 2.5f, 0.0f,
		-5.0f, 2.5f, 0.0f,

		-5.0f, -2.5f, 5.0f,
		5.0f, -2.5f, 5.0f,
		5.0f, 2.5f, 5.0f,

		-5.0f, -2.5f, 5.0f,
		5.0f, 2.5f, 5.0f,
		-5.0f, 2.5f, 5.0f,

		-5.0f, 2.5f, 0.0f,
		5.0f, 2.5f, 0.0f,
		5.0f, 2.5f, 5.0f,

		-5.0f, 2.5f, 0.0f,
		5.0f, 2.5f, 5.0f,
		-5.0f, 2.5f, 5.0f,


		-5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 5.0f,

		-5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 5.0f,
		-5.0f, -2.5f, 5.0f,


		5.0f, -2.5f, 0.0f,
		5.0f, 2.5f, 0.0f,
		5.0f, -2.5f, 5.0f,

		5.0f, -2.5f, 5.0f,
		5.0f, 2.5f, 0.0f,
		5.0f, 2.5f, 5.0f,


		-5.0f, -2.5f, 0.0f,
		-5.0f, 2.5f, 5.0f,
		-5.0f, -2.5f, 5.0f,

		-5.0f, -2.5f, 0.0f,
		-5.0f, 2.5f, 0.0f,
		-5.0f, 2.5f, 5.0f
	};

	//color for the shape A
	GLfloat a = 0.5f;
	static const GLfloat color1[] = {
		0.583f,  0.771f,  0.014f,a,
		0.583f,  0.771f,  0.014f,a,
		0.583f,  0.771f,  0.014f,a,
		0.583f,  0.771f,  0.014f,a,
		0.583f,  0.771f,  0.014f,a,
		0.583f,  0.771f,  0.014f,a,
		0.195f,  0.548f,  0.859f,a,
		0.195f,  0.548f,  0.859f,a,
		0.195f,  0.548f,  0.859f,a,
		0.195f,  0.548f,  0.859f,a,
		0.195f,  0.548f,  0.859f,a,
		0.195f,  0.548f,  0.859f,a,
		4.945f,  0.719f,  0.592f,a,
		4.945f,  0.719f,  0.592f,a,
		4.945f,  0.719f,  0.592f,a,
		4.945f,  0.719f,  0.592f,a,
		4.945f,  0.719f,  0.592f,a,
		4.945f,  0.719f,  0.592f,a,
		0.955f,  0.153f,  0.042f,a,
		0.955f,  0.153f,  0.042f,a,
		0.955f,  0.153f,  0.042f,a,
		0.955f,  0.153f,  0.042f,a,
		0.955f,  0.153f,  0.042f,a,
		0.955f,  0.153f,  0.042f,a,
		0.714f,  0.505f,  0.345f,a,
		0.714f,  0.505f,  0.345f,a,
		0.714f,  0.505f,  0.345f,a,
		0.714f,  0.505f,  0.345f,a,
		0.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
		1.714f,  0.505f,  0.345f,a,
	};

	//Shape B data
	static const GLfloat B[] =
	{
		//front
		2.5f, 0.0f, 2.5f,
		-2.5f, 0.0, 2.5f,
		2.5f, 14.0f, 2.5f,

		-2.5f, 0.0f, 2.5f,
		2.5f, 14.0f, 2.5f,
		-2.5f, 14.0f, 2.5f,
		//back
		-2.5f, 0.0f, -2.5f,
		2.5f, 0.0f, -2.5f,
		2.5f, 14.0f, -2.5f,

		-2.5f, 0.0f, -2.5f,
		2.5f, 14.0f, -2.5f,
		-2.5f, 14.0f, -2.5f,
		//right
		2.5f, 0.0f, -2.5f,
		2.5f, 0.0f, 2.5f,
		2.5f, 14.0f, -2.5f,

		2.5f, 0.0f, 2.5f,
		2.5f, 14.0f, -2.5f,
		2.5f, 14.0f, 2.5f,
		//left
		-2.5f, 0.0f, 2.5f,
		-2.5f, 0.0f, -2.5f,
		-2.5f, 14.0f, 2.5f,

		-2.5f, 0.0f, -2.5f,
		-2.5f, 14.0f, 2.5f,
		-2.5f, 14.0f, -2.5f,
		//up
		-2.5f, 14.0f, -2.5f,
		2.5f, 14.0f, -2.5f,
		2.5f, 14.0f, 2.5f,

		-2.5f, 14.0f, -2.5f,
		2.5f, 14.0f, 2.5f,
		-2.5f, 14.0f, 2.5f,
		//down
		-2.5f, 0.0f, -2.5f,
		2.5f, 0.0f, -2.5f,
		2.5f, 0.0f, 2.5f,

		-2.5f, 0.0f, -2.5f,
		2.5f, 0.0f, 2.5f,
		-2.5f, 0.0f, 2.5f
	};

	//Color for the shape B
	GLfloat b = 1.0f;
	static const GLfloat color2[] = {
		0.583f,  0.771f,  0.01f,b,
		0.583f,  0.771f,  0.01f,b,
		0.583f,  0.771f,  0.01f,b,
		0.583f,  0.771f,  0.01f,b,
		0.583f,  0.771f,  0.01f,b,
		0.583f,  0.771f,  0.01f,b,
		0.195f,  0.54f,  0.859f,b,
		0.195f,  0.54f,  0.859f,b,
		0.195f,  0.54f,  0.859f,b,
		0.195f,  0.54f,  0.859f,b,
		0.195f,  0.54f,  0.859f,b,
		0.195f,  0.54f,  0.859f,b,
		4.94f,  0.719f,  0.592f,b,
		4.94f,  0.719f,  0.592f,b,
		4.94f,  0.719f,  0.592f,b,
		4.94f,  0.719f,  0.592f,b,
		4.94f,  0.719f,  0.592f,b,
		4.94f,  0.719f,  0.592f,b,
		1.955f,  0.153f,  0.042f,b,
		1.955f,  0.153f,  0.042f,b,
		1.955f,  0.153f,  0.042f,b,
		1.955f,  0.153f,  0.042f,b,
		1.955f,  0.153f,  0.042f,b,
		1.955f,  0.153f,  0.042f,b,
		0.714f,  1.505f,  0.345f,b,
		0.714f,  1.505f,  0.345f,b,
		0.714f,  1.505f,  0.345f,b,
		0.714f,  1.505f,  0.345f,b,
		0.714f,  1.505f,  0.345f,b,
		1.714f,  1.505f,  0.345f,b,
		1.714f,  0.505f,  1.345f,b,
		1.714f,  0.505f,  1.345f,b,
		1.714f,  0.505f,  1.345f,b,
		1.714f,  0.505f,  1.345f,b,
		1.714f,  0.505f,  1.345f,b,
		1.714f,  0.505f,  1.345f,b,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint vertexbuffer1;
	glGenBuffers(1, &vertexbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(A), A, GL_STATIC_DRAW);

	GLuint colorbuffer1;
	glGenBuffers(1, &colorbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color1), color1, GL_STATIC_DRAW);

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(B), B, GL_STATIC_DRAW);

	GLuint colorbuffer2;
	glGenBuffers(1, &colorbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color2), color2, GL_STATIC_DRAW);

	float scalex = 1.0f;
	float scaley = 1.0f;
	float scalez = 1.0f;
	float rotatex = 0.0f;
	float rotatey = 0.0f;

	float zoom=1.0f;

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		// Use our shader
		glUseProgram(programID);


		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, 0.0f,30.0f), 
			glm::vec3(0.0f, 0.0f, 2.5f), //look at point E
			glm::vec3(0.0f, 1.0f, 0.0f) 
		);
		glm::mat4 Model = glm::mat4(1.0f);



	

		
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			scalex += 0.01f;
		}
		else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			scalex -= 0.01f;
		}
		Model = glm::scale(Model, glm::vec3(scalex, 1.0f, 1.0f));

		
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			scaley += 0.01f;
		}
		else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			scaley -= 0.01f;
		}
		Model = glm::scale(Model, glm::vec3(1.0f, scaley, 1.0f));

		
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			scalez += 0.01f;
		}
		else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			scalez -= 0.01f;
		}
		Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, scalez));




		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			rotatex += 0.5;
			View = glm::rotate(View, glm::radians(rotatex), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			rotatex -= 0.5;
			View = glm::rotate(View, glm::radians(rotatex), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		View = glm::rotate(View, glm::radians(rotatex), glm::vec3(1.0f, 0.0f, 0.0f));
		
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			rotatey += 0.5;
			View = glm::rotate(View, glm::radians(rotatey), glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			rotatey -= 0.5;
			View = glm::rotate(View, glm::radians(rotatey), glm::vec3(0.0f, 1.0f, 0.0f));

		}
		View = glm::rotate(View, glm::radians(rotatey), glm::vec3(0.0f, 1.0f, 0.0f))
		
		if (glfwGetKey(window,GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			zoom += 0.01f;
			View = glm::scale(View, glm::vec3(zoom, zoom, zoom));

		}

		else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			zoom -= 0.01f;
			View = glm::scale(View, glm::vec3(zoom, zoom, zoom));

		}
		View = glm::scale(View, glm::vec3(zoom, zoom, zoom));


		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		// Cube buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Cube buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles

		
		
		//  Shape A buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Shape A buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles


		// Shape B buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Shape B buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles

	
		glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the Space key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &vertexbuffer1);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

