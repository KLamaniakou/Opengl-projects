//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1Α της OpenGL
//
//ΑΜ:5110                         Όνομα:Κωνσταντινος-Διονυσιος Λαμανιακου
//ΑΜ:4993                         Όνομα:Ιωάννης Τσόχλας

//*********************************

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <windows.h> //Για να μπορεσουμε να χρησιμοποιησουμε την συναρτηση Sleep..
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

//******************
// Η LoadShaders είναι black box για σας

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

void square(GLuint vb, GLuint vb2) {
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

	glBindBuffer(GL_ARRAY_BUFFER, vb2);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();

	for (int i = 0; i < 6; i++) {
		cout << i << "\n";
	}
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context

	window = glfwCreateWindow(900, 900, u8"Πρώτη Ασκηση 2023", NULL, NULL);
	//εδω ορισαμε το παραθυρο να ειναι 900χ900 και δινουμε τιτλο στο παραθυρο δηλαδη το υποερωτημα 1..


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

	// blue background
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//***********************************************
	// Οι shaders σας είναι οι 
	// ProjectVertexShader.vertexshader
	// ProjectFragmentShader.fragmentshader

	GLuint programID = LoadShaders("ProjectVertexShader.vertexshader", "ProjectFragmentShader.fragmentshader");

	///////////////////////////////////////////////////////////////////////////////////////	
		/**Το παρακάτω το αγνοείτε - είναι τοποθέτηση κάμερας ***/
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 30), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	///////////////////////////////////////////////////////////////////////////////////////
		//**************************************************
		/// Για βοήθεια το πρόγραμμα αναπαριστά ενα τυχαίο τρίγωνο - εσείς θα πρέπει να βάλετε κορυφές κατάλληλες 
		//  για το δικό σας τρίγωνο.
		//Στην άσκηση αυτή δουλεύετε στις 2 διαστάσεις x,y οπότε η z συνιστώσα θα ειναι πάντα 0.0f

	static const GLfloat shape_1_buffer[] = {
		 -1.5f, -1.5f, 0.0f,
		 1.5f,  1.5f, 0.0f,
		 1.5f ,-1.5f,0.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_1_buffer), shape_1_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_2_buffer[] = {
		 -1.5f, -1.5f, 0.0f,
		1.5f,  1.5f, 0.0f,
		-1.5f,1.5f,0.0f
	};

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_2_buffer), shape_2_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_3_buffer[] = {
	 1.5f, 1.5f, 0.0f,
	 4.5f,  4.5f, 0.0f,
	 1.5f ,4.5f,0.0f
	};

	GLuint vertexbuffer3;
	glGenBuffers(1, &vertexbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_3_buffer), shape_3_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_4_buffer[] = {
		 1.5f, 1.5f, 0.0f,
		4.5f,  4.5f, 0.0f,
		4.5f,1.5f,0.0f
	};

	GLuint vertexbuffer4;
	glGenBuffers(1, &vertexbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_4_buffer), shape_4_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_5_buffer[] = {
		-1.5f, -1.5f, 0.0f,
		-4.5f,  -4.5f, 0.0f,
		-1.5f ,-4.5f,0.0f
	};

	GLuint vertexbuffer5;
	glGenBuffers(1, &vertexbuffer5);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_5_buffer), shape_5_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_6_buffer[] = {
		-1.5f, -1.5f, 0.0f,
		-4.5f,  -4.5f, 0.0f,
		-4.5f,-1.5f,0.0f
	};

	GLuint vertexbuffer6;
	glGenBuffers(1, &vertexbuffer6);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_6_buffer), shape_6_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_7_buffer[] = {
		-1.5f, 4.5f, 0.0f,
		-4.5f,  1.5f, 0.0f,
		-1.5f ,1.5f,0.0f
	};

	GLuint vertexbuffer7;
	glGenBuffers(1, &vertexbuffer7);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_7_buffer), shape_7_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_8_buffer[] = {
		-1.5f, 4.5f, 0.0f,
		-4.5f,  1.5f, 0.0f,
		-4.5f,4.5f,0.0f
	};
	GLuint vertexbuffer8;
	glGenBuffers(1, &vertexbuffer8);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_8_buffer), shape_8_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_9_buffer[] = {
		4.5f, -1.5f, 0.0f,
		1.5f,  -4.5f, 0.0f,
		1.5f ,-1.5f,0.0f
	};

	GLuint vertexbuffer9;
	glGenBuffers(1, &vertexbuffer9);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_9_buffer), shape_9_buffer, GL_STATIC_DRAW);

	static const GLfloat shape_10_buffer[] = {
		4.5f, -1.5f, 0.0f,
		1.5f,  -4.5f, 0.0f,
		4.5f,-4.5f,0.0f
	};
	GLuint vertexbuffer10;
	glGenBuffers(1, &vertexbuffer10);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_10_buffer), shape_10_buffer, GL_STATIC_DRAW);
	int j = 1; //ο μετρητης για το μοτιβο

	int time = 300; //ο χρονος για την αναλλαγει του μοτιβου
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
		// Use our shader
		glUseProgram(programID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);  /// Αυτό αφορά την κάμερα  - το αγνοείτε
		if (j == 1 or j == 3 or j == 5 or j == 7) {
			square(vertexbuffer, vertexbuffer2);
			Sleep(time);
			j++;
		}
		else if (j == 2 ) {
			square(vertexbuffer3, vertexbuffer4);
			Sleep(time);
			j++;
		}
		else if (j == 4) {
			square(vertexbuffer5, vertexbuffer6);
			Sleep(time);
			j++;
		}
		else if (j == 6) {
			square(vertexbuffer7, vertexbuffer8);
			Sleep(time);
			j++;
		}
		else if (j == 8) {
			square(vertexbuffer9, vertexbuffer10);
			Sleep(time);
			j = j - 7;
		}
		if (glfwGetKey(window, GLFW_KEY_U) != GLFW_PRESS) {
			time = time + 20;
		}
		if(glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
			time = time - 20;
		}
	} while (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	//αλλαξαμε το πληκτρο σε C οπως ζηταει το ερωτημα 1 για να κλεινει το παραθυρο 

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &vertexbuffer3);
	glDeleteBuffers(1, &vertexbuffer4);
	glDeleteBuffers(1, &vertexbuffer5);
	glDeleteBuffers(1, &vertexbuffer6);
	glDeleteBuffers(1, &vertexbuffer7);
	glDeleteBuffers(1, &vertexbuffer8);
	glDeleteBuffers(1, &vertexbuffer9);
	glDeleteBuffers(1, &vertexbuffer10);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}