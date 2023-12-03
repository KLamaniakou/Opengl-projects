//********************************
// 2023
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1C της OpenGL
//
//ΑΜ: 5110                   Όνομα:Κωνσταντίνος-Διονύσιος Λαμανιακου
//ΑΜ: 4993                   Όνομα:Ιωάννης Τσόχλας

//*********************************
	#define _CRT_SECURE_NO_WARNINGS

	#define STB_IMAGE_IMPLEMENTATION

	// Include standard headers
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <vector>
	#include <iostream>
	#include <fstream>
	#include <algorithm>
	#include <sstream>
	#include "stb_image.h"

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

	GLuint loadT(const char* name);


	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::mat4 getViewMatrix() {
		return ViewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return ProjectionMatrix;
	}

//************************************
// Η LoadShaders είναι black box για σας
//************************************
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(950, 950, u8"Εργασία 1Γ – CSG Boolean Operations", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Dark grey color
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1C.vertexshader", "P1C.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

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
		-5.0f, 2.5f, 5.0f,
	};
	static const GLfloat uvA[] =
	{
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			1.0, 0.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			0.0, 1.0,

	};
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
		-2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 5.0f,
		2.5f, 14.0f, 5.0f,

		-2.5f, 0.0f, 5.0f,
		2.5f, 14.0f, 5.0f,
		-2.5f, 14.0f, 5.0f,
		//right
		2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 0.0f,
		2.5f, 14.0f, 5.0f,

		2.5f, 0.0f, 0.0f,
		2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 0.0f,
		//left
		-2.5f, 0.0f, 0.0f,
		-2.5f, 0.0f, 5.0f,
		-2.5f, 14.0f, 0.0f,

		-2.5f, 0.0f, 5.0f,
		-2.5f, 14.0f, 0.0f,
		-2.5f, 14.0f, 5.0f,
		//up
		-2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 0.0f,

		-2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 0.0f,
		-2.5f, 14.0f, 0.0f,
		//down
		-2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 0.0f,

		-2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 0.0f,
		-2.5f, 0.0f, 0.0f
	};

	static const GLfloat uvB[] =
	{
		// front
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	// back
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	// right
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	// left
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	// up
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	// down
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
	};
	static const GLfloat T[] =
	{
		//front
		2.5f, 0.0f, 2.5f,
		-2.5f, 0.0, 2.5f,
		2.5f, 2.5f, 2.5f,

		-2.5f, 0.0f, 2.5f,
		2.5f, 2.5f, 2.5f,
		-2.5f, 2.5f, 2.5f,
		//back
		-2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 5.0f,
		2.5f, 2.5f, 5.0f,

		-2.5f, 0.0f, 5.0f,
		2.5f, 2.5f, 5.0f,
		-2.5f, 2.5f, 5.0f,
		//right
		2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 0.0f,
		2.5f, 2.5f, 5.0f,

		2.5f, 0.0f, 0.0f,
		2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 0.0f,
		//left
		-2.5f, 0.0f, 0.0f,
		-2.5f, 0.0f, 5.0f,
		-2.5f, 2.5f, 0.0f,

		-2.5f, 0.0f, 5.0f,
		-2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 5.0f,
		//up
		-2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 0.0f,

		-2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,
		//down
		-2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 0.0f,

		-2.5f, 0.0f, 5.0f,
		2.5f, 0.0f, 0.0f,
		-2.5f, 0.0f, 0.0f
	};

	static const GLfloat uvT[] =
	{
		//front
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//back
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//right
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//left
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//up
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		//down
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	static const GLfloat D[] =
	{
		//front
		-5.0f, -2.5f, 0.0f,
		-2.5f,0.0f, 0.0f,
		-2.5f, 2.5f, 0.0f,

		-5.0f, -2.5f, 0.0f,
		-5.0f,2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,

		-5.0f, -2.5f, 0.0f,
		-2.5f,0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		-5.0f, -2.5f, 0.0f,
		0.0f,0.0f, 0.0f,
		0.0f,-2.5f, 0.0f,

		5.0f, 2.5f, 0.0f,
		2.5f,0.0f, 0.0f,
		2.5f, 2.5f, 0.0f,

		5.0f, -2.5f, 0.0f,
		5.0f,2.5f, 0.0f,
		2.5f,0.0f, 0.0f,

		5.0f, -2.5f, 0.0f,
		2.5f,0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		5.0f, -2.5f, 0.0f,
		0.0f,0.0f, 0.0f,
		0.0f,-2.5f, 0.0f,

		//back
		-5.0f, -2.5f, 5.0f,
		-2.5f,0.0f, 5.0f,
		-2.5f, 2.5f, 5.0f,

		-5.0f, -2.5f, 5.0f,
		-5.0f,2.5f, 5.0f,
		-2.5f, 2.5f, 5.0f,

		-5.0f, -2.5f, 5.0f,
		-2.5f,0.0f, 5.0f,
		0.0f, 0.0f, 5.0f,

		-5.0f, -2.5f, 5.0f,
		0.0f,0.0f, 5.0f,
		0.0f,-2.5f, 5.0f,

		5.0f, 2.5f, 5.0f,
		2.5f,0.0f, 5.0f,
		2.5f, 2.5f, 5.0f,

		5.0f, -2.5f, 5.0f,
		5.0f,2.5f, 5.0f,
		2.5f,0.0f, 5.0f,

		5.0f, -2.5f, 5.0f,
		2.5f,0.0f, 5.0f,
		0.0f, 0.0f, 5.0f,

		5.0f, -2.5f, 5.0f,
		0.0f,0.0f, 5.0f,
		0.0f,-2.5f, 5.0f,
		//left-right
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
		-5.0f, 2.5f, 5.0f,
		//down
		-5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 5.0f,

		-5.0f, -2.5f, 0.0f,
		5.0f, -2.5f, 5.0f,
		-5.0f, -2.5f, 5.0f,
		//up
		-5.0,2.5,0.0,
		-2.5,2.5,5.0,
		-2.5,2.5,0.0,
		-5.0,2.5,0.0,
		-2.5,2.5,5.0,
		-5.0,2.5,5.0,

		5.0,2.5,0.0,
		2.5,2.5,5.0,
		2.5,2.5,0.0,
		5.0,2.5,0.0,
		2.5,2.5,5.0,
		5.0,2.5,5.0,

		//in side up
	   -2.5,2.5,0.0,
	   -2.5,0.0,0.0,
	   -2.5,0.0,5.0,
	   -2.5,2.5,0.0,
	   -2.5,2.5,5.0,
	   -2.5,0.0,5.0,

	   2.5, 2.5, 0.0,
	   2.5, 0.0, 0.0,
	   2.5, 0.0, 5.0,
	   2.5, 2.5, 0.0,
	   2.5, 2.5, 5.0,
	   2.5, 0.0, 5.0,

	   //center up
	   -2.5,0.0,0.0,
	   2.5,0.0,0.0,
	   2.5,0.0,5.0,
	   -2.5, 0.0, 0.0,
	   -2.5, 0.0, 5.0,
	   2.5, 0.0, 5.0,
	};
	static const GLfloat uvD[] = {
	   // front
	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   0.0f, 1.0f,

	   // back
	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   0.0f, 1.0f,

	   // left-right
	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   0.0f, 1.0f,

	   1.0f, 0.0f,
	   1.0f, 1.0f,
	   0.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   0.0f, 1.0f,

	   1.0f, 0.0f,
	   1.0f, 1.0f,
	   0.0f, 1.0f,

	   // down
	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   0.0f, 1.0f,

	   // up
	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 0.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   // inside up
	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,

	   // center up
	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,
	   0.0f, 0.0f,
       0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,
	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,

	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 0.0f,
	};
	static const GLfloat D2[] =
	{
		//front
		2.5f, 2.5f, 2.5f,
		-2.5f,2.5f, 2.5f,
		2.5f, 14.0f, 2.5f,

		-2.5f,2.5f, 2.5f,
		2.5f, 14.0f, 2.5f,
		-2.5f, 14.0f, 0.0f,
		//back
		-2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 5.0f,
		2.5f, 14.0f, 5.0f,

		-2.5f, 2.5f, 5.0f,
		2.5f, 14.0f, 5.0f,
		-2.5f, 14.0f, 5.0f,
		//right
		2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 0.0f,
		2.5f, 14.0f, 5.0f,

		2.5f, 2.5f, 0.0f,
		2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 0.0f,
		//left
		-2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 5.0f,
		-2.5f, 14.0f, 0.0f,

		-2.5f, 2.5f, 5.0f,
		-2.5f, 14.0f, 0.0f,
		-2.5f, 14.0f, 5.0f,
		//up
		-2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 0.0f,

		-2.5f, 14.0f, 5.0f,
		2.5f, 14.0f, 0.0f,
		-2.5f, 14.0f, 0.0f,
		//down
		-2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 0.0f,

		-2.5f, 2.5f, 5.0f,
		2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f
	};


	static const GLfloat uvD2[] =
	{
		//front
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//back
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//right
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//left
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//up
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//down
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	GLuint vertexbuffer1;
	glGenBuffers(1, &vertexbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(A), A, GL_STATIC_DRAW);

	GLuint uvbuffer1;
	glGenBuffers(1, &uvbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvA), uvA, GL_STATIC_DRAW);

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(B), B, GL_STATIC_DRAW);


	GLuint uvbuffer2;
	glGenBuffers(1, &uvbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvB), uvB, GL_STATIC_DRAW);

	GLuint vertexbuffer3;
	glGenBuffers(1, &vertexbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(T), T, GL_STATIC_DRAW);

	GLuint uvbuffer3;
	glGenBuffers(1, &uvbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvT), uvT, GL_STATIC_DRAW);

	GLuint vertexbuffer4;
	glGenBuffers(1, &vertexbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(D), D, GL_STATIC_DRAW);

	GLuint uvbuffer4;
	glGenBuffers(1, &uvbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvD), uvD, GL_STATIC_DRAW);

	GLuint vertexbuffer5;
	glGenBuffers(1, &vertexbuffer5);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(D2), D2, GL_STATIC_DRAW);

	GLuint uvbuffer5;
	glGenBuffers(1, &uvbuffer5);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvD2), uvD2, GL_STATIC_DRAW);

	float rx = 0.0f;
	float ry = 0.0f;
	float z = 1.0f;

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 40.0f),
			glm::vec3(0.0f, 0.0f, 2.5f), 
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		glm::mat4 Model = glm::mat4(1.0f);


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			rx += 0.5;
			View = glm::rotate(View, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			rx -= 0.5;
			View = glm::rotate(View, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		View = glm::rotate(View, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			ry += 0.5;
			View = glm::rotate(View, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			ry -= 0.5;
			View = glm::rotate(View, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));

		}
		View = glm::rotate(View, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			z += 0.01f;
			View = glm::scale(View, glm::vec3(z, z, z));

		}
		else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			z -= 0.01f;
			View = glm::scale(View, glm::vec3(z, z, z));

		}
		View = glm::scale(View, glm::vec3(z, z, z));


		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
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
			glBindTexture(GL_TEXTURE_2D, loadT("texture.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles
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
			glBindTexture(GL_TEXTURE_2D, loadT("textureA1.jpg"));

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangle
		}
		else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			glBindTexture(GL_TEXTURE_2D, loadT("textureA1.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles

		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			glBindTexture(GL_TEXTURE_2D, loadT("textureA1.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer4);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 200); // 3 indices starting at 0 -> 12 triangles
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			glBindTexture(GL_TEXTURE_2D, loadT("texture.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer5);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles
		}
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {

			rx = 0.0;
			View = glm::rotate(View, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));

			ry = 0.0;
			View = glm::rotate(View, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));

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

			glBindTexture(GL_TEXTURE_2D, loadT("texture.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles

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

			glBindTexture(GL_TEXTURE_2D, loadT("textureA1.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangle
			}
		else {

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

			glBindTexture(GL_TEXTURE_2D, loadT("texture.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangles

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

			glBindTexture(GL_TEXTURE_2D, loadT("textureA1.jpg"));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 12 triangle
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer1);
	glDeleteBuffers(1, &uvbuffer1);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &uvbuffer2);
	glDeleteBuffers(1, &vertexbuffer3);
	glDeleteBuffers(1, &uvbuffer3);
	glDeleteBuffers(1, &vertexbuffer4);
	glDeleteBuffers(1, &uvbuffer4);
	glDeleteBuffers(1, &vertexbuffer5);
	glDeleteBuffers(1, &uvbuffer5);

	

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;

};
GLuint loadT(const char* name)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textureID, 0);

	int width, height, chan;
	unsigned char* data = stbi_load(name, &width, &height, &chan, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	return textureID;
}