#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

using namespace std;

// GLM
#include <glm/fwd.hpp> //gebruik om glm sneller te laten compileren
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //glm gedeelte over matrix transformaties

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

/*
http://learnopengl.com/#!Getting-started/Hello-Triangle
*/

struct asset {
    GLuint shaders;
    GLuint vbo;
    GLuint vao;
    GLenum drawType;
    GLint drawStart;
    GLint drawCount;
};

struct instance {
    asset* myAsset = nullptr;
    //glm::mat4 transform;
};

asset* triangle;

int windowWidth, windowHeight;

//glm::mat4 cameraPosition;
//glm::mat4 projection;
//glm::mat4 camera;

// Is called whenever a key is pressed/released via GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
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
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

asset* loadTriangle() {
    if (triangle == nullptr) {
        cout << "loading triangle..." << endl;

        triangle = new asset;

        // Build and compile our shader program
        triangle->shaders = LoadShaders("vertexshader.glsl", "fragmentShader.glsl");

        triangle->drawType = GL_TRIANGLES;
        triangle->drawStart = 0;
        triangle->drawCount = 3;

        // Set up our vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
          -0.5f, -0.5f,     //Left (X,Y)
          0.5f, -0.5f,      //Right (X,Y)
          0.0f, 0.5f        //Top (X,Y)
        };

        glGenVertexArrays(1, &(triangle->vao));
        glGenBuffers(1, &(triangle->vbo));

        // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
        glBindVertexArray(triangle->vao);

        glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    }
    return triangle;
}

void deleteTriangle() {
    cout << "deleting triangle..." << endl;
    delete triangle;
    triangle = nullptr;
}

void displayInstance(const instance& tempInstance) {
    //cout << "displaying an instance..." << endl;

    const asset& tempAsset = *(tempInstance.myAsset);

    // Draw our first triangle
    glUseProgram(tempAsset.shaders);
    glBindVertexArray(tempAsset.vao);
    glDrawArrays(tempAsset.drawType, tempAsset.drawStart, tempAsset.drawCount);

    glBindVertexArray(0);
}

// The MAIN function, from here we start our application and run our Program/Game loop
int main() {
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Set the required callback functions
    glfwSetKeyCallback(window, keyCallback);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, windowWidth, windowHeight);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    instance triangleObject;
    triangleObject.myAsset = loadTriangle();


    // Game loop
    while(!glfwWindowShouldClose(window))     {
        // Check and call events
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        displayInstance(triangleObject);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    deleteTriangle();

    glfwTerminate();
    return 0;
}

