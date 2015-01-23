#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846
#define phi 1.6180339887498948482

using namespace std;

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //glm gedeelte over matrix transformaties
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

/*
http://learnopengl.com/#!Getting-started/Hello-Triangle
http://learnopengl.com/#!Getting-started/Shaders
*/

struct asset {
    GLuint shaders;
    GLuint MVPid;
    GLuint vbo;
    GLuint vao;
    GLenum drawType;
    GLint drawStart;
    GLint drawCount;
};

struct instance {
    asset* myAsset = nullptr;
    glm::mat4 transform = glm::mat4(1.0f);
};

asset* triangle;
asset* sphere;

int windowWidth = 800, windowHeight = 600;

glm::mat4 view = glm::mat4(1.0f);         //the position and orientation of the camera
glm::mat4 projection = glm::mat4(1.0f);   //the shape of the view
glm::mat4 camera = glm::mat4(1.0f);       //projection * view
glm::mat4 MVPmatrix = glm::mat4(1.0f);    //instance.transform * camera, thus different for every instance

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
        triangle->drawCount = 1 * 3;

        // Set up our vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
          -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.0f, 0.5f, 0.0f
        };

        glGenVertexArrays(1, &(triangle->vao));
        glGenBuffers(1, &(triangle->vbo));

        // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
        glBindVertexArray(triangle->vao);

        glBindBuffer(GL_ARRAY_BUFFER, triangle->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(12, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(12);

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    }
    return triangle;
}

void deleteTriangle() {
    cout << "deleting triangle..." << endl;
    delete triangle;
    triangle = nullptr;
}

asset* loadSphere() {
    if (sphere == nullptr) {
        cout << "loading sphere..." << endl;

        sphere = new asset;

        // Build and compile our shader program
        sphere->shaders = LoadShaders("vertexshader.glsl", "fragmentShader.glsl");

        sphere->drawType = GL_TRIANGLES;
        sphere->drawStart = 0;
        sphere->drawCount = 20 * 3;

        // Set up our vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
            //dakjes:
            //(0, �1, �phi)
            0.0f, 1.0f, phi,
            0.0f, -1.0f, phi,
            phi, 0.0f, 1.0f,

            0.0f, 1.0f, phi,
            0.0f, -1.0f, phi,
            -phi, 0.0f, 1.0f,

            0.0f, -1.0f, -phi,
            0.0f, 1.0f, -phi,
            -phi, 0.0f, -1.0f,

            0.0f, -1.0f, -phi,
            0.0f, 1.0f, -phi,
            phi, 0.0f, -1.0f,

            //(�1, �phi, 0)
            1.0f, phi, 0.0f,
            -1.0f, phi, 0.0f,
            0.0f, 1.0f, phi,

            1.0f, phi, 0.0f,
            -1.0f, phi, 0.0f,
            0.0f, 1.0f, -phi,

            -1.0f, -phi, 0.0f,
            1.0f, -phi, 0.0f,
            0.0f, -1.0f, -phi,

            -1.0f, -phi, 0.0f,
            1.0f, -phi, 0.0f,
            0.0f, -1.0f, phi,

            //(�phi, 0, �1)
            phi, 0.0f, 1.0f,
            phi, 0.0f, -1.0f,
            1.0f, phi, 0.0f,

            phi, 0.0f, 1.0f,
            phi, 0.0f, -1.0f,
            1.0f, -phi, 0.0f,

            -phi, 0.0f, -1.0f,
            -phi, 0.0f, 1.0f,
            -1.0f, -phi, 0.0f,

            -phi, 0.0f, -1.0f,
            -phi, 0.0f, 1.0f,
            -1.0f, phi, 0.0f,

            //hoekjes:
        //origineel:
            0.0f, 1.0f, phi,
            1.0f, phi, 0.0f,
            phi, 0.0f, 1.0f,

        //gespiegeld om X-as:
            0.0f, 1.0f, phi,
            -1.0f, phi, 0.0f,
            -phi, 0.0f, 1.0f,

        //gespiegeld om Y-as:
            0.0f, -1.0f, phi,
            1.0f, -phi, 0.0f,
            phi, 0.0f, 1.0f,

            0.0f, -1.0f, phi,
            -1.0f, -phi, 0.0f,
            -phi, 0.0f, 1.0f,

        //gespiegeld om Z-as:
            0.0f, 1.0f, -phi,
            1.0f, phi, 0.0f,
            phi, 0.0f, -1.0f,

            0.0f, 1.0f, -phi,
            -1.0f, phi, 0.0f,
            -phi, 0.0f, -1.0f,

            0.0f, -1.0f, -phi,
            1.0f, -phi, 0.0f,
            phi, 0.0f, -1.0f,

            0.0f, -1.0f, -phi,
            -1.0f, -phi, 0.0f,
            -phi, 0.0f, -1.0f
        };

        glGenVertexArrays(1, &(sphere->vao));
        glGenBuffers(1, &(sphere->vbo));

        // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
        glBindVertexArray(sphere->vao);

        glBindBuffer(GL_ARRAY_BUFFER, sphere->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(12, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(12);

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    }
    return sphere;
}

void deleteSphere() {
    cout << "deleting sphere..." << endl;
    delete sphere;
    triangle = nullptr;
}

void displayInstance(const instance& tempInstance) {
    const asset& tempAsset = *(tempInstance.myAsset);

    glUseProgram(tempAsset.shaders);    //open our shaders
    glBindVertexArray(tempAsset.vao);   //use our buffers and information stored with it

    GLuint MVPid = glGetUniformLocation(tempAsset.shaders, "MVP");
    MVPmatrix = camera * tempInstance.transform;
    glUniformMatrix4fv(MVPid, 1, GL_FALSE, glm::value_ptr(MVPmatrix));

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

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
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

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    // Accept fragment if it closer to the camera than the former one
    glClearDepth(100.0f);           //zorg dat deze NIET lager is dan de far clipping plane


    projection = glm::perspective(70.0f, windowWidth / (float)windowHeight, 0.1f, 100.0f);

    glm::mat4 tempMatrix;

    instance myInstance;
    myInstance.myAsset = loadSphere();

    tempMatrix = glm::scale(tempMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    myInstance.transform = tempMatrix;

    tempMatrix = glm::translate(tempMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
    myInstance.transform = tempMatrix * myInstance.transform;

    instance mySphere;
    mySphere.myAsset = loadSphere();
    mySphere.transform = glm::translate(mySphere.transform, glm::vec3(-2.0f, 0.0f, 0.0f));

    double beginTime = glfwGetTime();
    double currentTime;

    // Game loop
    while(!glfwWindowShouldClose(window))     {
        // Check and call events
        glfwPollEvents();

        // Clear the colorbuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update
        currentTime = glfwGetTime() - beginTime;

        view = glm::lookAt(glm::vec3(cos(currentTime * PI / 2) * 6, 2.0f + 2.0f * sin (currentTime * PI / 6.5f), sin(currentTime * PI / 2) * 6),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f));


        // Render
        camera = projection * view;

        displayInstance(myInstance);
        displayInstance(mySphere);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    deleteTriangle();
    deleteSphere();

    glfwTerminate();
    return 0;
}

