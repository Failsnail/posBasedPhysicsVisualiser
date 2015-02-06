#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

#define fullscreen false

#define PI 3.14159265358979323846
#define phi 1.6180339887498948482
#define root2 1.4142135623730950488

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

#include "posBased/simulator.h"
#include "posBased/worldstate.h"
#include "posBased/particles/particle.h"
#include "posBased/particles/particlepool.h"
#include "posBased/constraints/constraint.h"
#include "posBased/constraints/distanceconstraint.h"
#include "posBased/constraints/positionconstraint.h"
#include "posBased/softforces/softforce.h"
#include "posBased/vector/vector2d.h"
#include "posBased/vector/vector3d.h"
#include "posBased/softforces/gravity.h"
//#include "posBased/softforces/spring.h" //NOT FUNCTIONAL
//#include "posBased/softforces/damper.h" //NOT FUNCTIONAL

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
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1;
};

asset* triangle;
asset* sphere;

double beginTime, currentTime, lastTime, deltaTime;
double otherTime = 0;

float timeScale = 1;
bool paused = false;

double lastXmouse, lastYmouse;
double Xmouse, Ymouse;
double yaw = -0.5f * PI, pitch = 0;

int windowWidth = 800, windowHeight = 600;
GLFWwindow* window;

bool keys[1024];

glm::vec3 cameraPos = glm::vec3(0.0f, -1.0f, 20.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view = glm::mat4(1.0f);         //the position and orientation of the camera
glm::mat4 projection = glm::mat4(1.0f);   //the shape of the view
glm::mat4 camera = glm::mat4(1.0f);       //projection * view
glm::mat4 MVPmatrix = glm::mat4(1.0f);    //instance.transform * camera, thus different for every instance

int particle1, particle2, particle3, particle4, particle5, particle6, particle7, particle8, particle9, particle10,
particle11, particle12, particle13, particle14, particle15, particle16, particle17, particle18, particle19;

worldstate myWorldstate;
simulator mySimulator;

instance** instanceList = nullptr;
int instanceListLength = 0;


// Is called whenever a key is pressed/released via GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if(action == GLFW_PRESS) {
            keys[key] = true;
            if (key == GLFW_KEY_UP) {
                timeScale *= 2;
            } else if (key == GLFW_KEY_DOWN) {
                timeScale /= 2;
            } else if (key == GLFW_KEY_P) {
                paused = !paused;
            } else if (key == GLFW_KEY_R) {
                timeScale = 1.0f;
            }

        } else if(action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Xmouse = xpos;
    Ymouse = ypos;
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
            //(0, ±1, ±phi)
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

            //(±1, ±phi, 0)
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

            //(±phi, 0, ±1)
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

void displayInstance(const instance& newInstance) {
    if (newInstance.myAsset == nullptr) {
        return; //the instance didn't contain an asset
    }
    const asset& tempAsset = *(newInstance.myAsset);

    glUseProgram(tempAsset.shaders);    //open our shaders
    glBindVertexArray(tempAsset.vao);   //use our buffers and information stored with it

    GLuint MVPid = glGetUniformLocation(tempAsset.shaders, "MVP");

    glm::mat4 transform;

    {
    glm::mat4 translate;
    glm::mat4 scale;
    translate = glm::translate(translate, newInstance.position);
    scale = glm::scale(scale, glm::vec3(newInstance.scale, newInstance.scale, newInstance.scale));
    transform = translate * scale;
    }

    MVPmatrix = camera * transform;

    glUniformMatrix4fv(MVPid, 1, GL_FALSE, glm::value_ptr(MVPmatrix));

    glDrawArrays(tempAsset.drawType, tempAsset.drawStart, tempAsset.drawCount);

    glBindVertexArray(0);
}

void moveCamera() {
    //update camera parameters to mouse input
    GLfloat sensitivity = 0.001f;
    yaw += (Xmouse - lastXmouse) * sensitivity;
    pitch -= (Ymouse - lastYmouse) * sensitivity;

    if (pitch < -0.49 * PI) {
        pitch = -0.49 * PI;
    }
    if (pitch > 0.49 * PI) {
        pitch = 0.49 * PI;
    }

    cameraDir.x = cos(yaw) * cos(pitch);
    cameraDir.y = sin(pitch);
    cameraDir.z = sin(yaw) * cos(pitch);

    cameraDir = glm::normalize(cameraDir);

    lastXmouse = Xmouse;
    lastYmouse = Ymouse;

    //update camera parameters to keyboard input
    GLfloat cameraSpeed = 0.05f;
    if (keys[GLFW_KEY_W]) {
        //cameraPos += cameraSpeed * cameraDir;
        cameraPos += cameraSpeed * glm::normalize(glm::vec3(cameraDir.x, 0.0f, cameraDir.z));
    }
    if (keys[GLFW_KEY_S]) {
        //cameraPos -= cameraSpeed * cameraDir;
        cameraPos -= cameraSpeed * glm::normalize(glm::vec3(cameraDir.x, 0.0f, cameraDir.z));
    }
    if (keys[GLFW_KEY_A]) {
        cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
    }
    if (keys[GLFW_KEY_D]) {
        cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
    }
    if (keys[GLFW_KEY_SPACE]) {
        cameraPos.y += cameraSpeed;
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        cameraPos.y -= cameraSpeed;
    }
    //update the view matrix to the new parameters
    view = glm::lookAt(cameraPos,
                       cameraPos + cameraDir,
                       cameraUp);
}

glm::vec3 convertVector(const vector2D& newVector) {
    return glm::vec3(newVector.getX(), newVector.getY(), 0.0f);
}

glm::vec3 convertVector(const vector3D& newVector) {
    return glm::vec3(newVector.getX(), newVector.getY(), newVector.getZ());
}

void deleteInstanceList() {
    for (int i = 0; i < instanceListLength; i++) {
        delete instanceList[i];
    }
    delete instanceList;
    instanceList = nullptr;
    instanceListLength = 0;
}

void initialezeInstanceList(const int& newLength) {
    //if the new length isn't the old length, delete the list and make a one with the good size
    if (instanceListLength != newLength) {
        cout << "wiping instanceList to reinitialize..." << endl;
        deleteInstanceList();
        cout << "initializing instanceList..." << endl;
        instanceListLength = newLength;
        instanceList = new instance* [instanceListLength];
        //make shure the list only contains nullptrs
        cout << "clearing instanceList..." << endl;
        for (int i = 0; i < instanceListLength; i++) {
            instanceList[i] = nullptr;
        }
    } else {
        cout << "clearing InstanceList..." << endl;
        //delete any instances stored with the list and set them to nullptrs
        for (int i = 0; i < instanceListLength; i++) {
            if (instanceList[i] != nullptr) {
                delete instanceList[i];
                instanceList[i] = nullptr;
            }
        }
    }
}

void setInstanceList(const worldstate& newWorldState) {
    cout << "getting a particlePool&..." << endl;
    const particlePool& tempParticlePool = newWorldState.getParticlePool();

    cout << "preparing the instanceList..." << endl;
    initialezeInstanceList(tempParticlePool.getParticlePoolSize());

    cout << "copying values..." << endl;
    for (int i = 0; i < instanceListLength; i++) {
        if (tempParticlePool.isActive(i)) {
            instanceList[i] = new instance;
            instanceList[i]->scale = 0.25f;
            instanceList[i]->myAsset = loadSphere();
            instanceList[i]->position = convertVector(tempParticlePool.getPosition(i));
        }
    }
}

void displayInstanceList() {
    if (instanceList != nullptr) {
        for (int i = 0; i < instanceListLength; i++) {
            if (instanceList[i] != nullptr) {
                displayInstance(*(instanceList[i]));
            }
        }
    }
}

void loadWorld() {

    mySimulator.setFullIterationsNumber(10);             //50
    mySimulator.setRelaxationIterationsNumber(3);       //3
    mySimulator.setRelaxationCoefficient(0.3f);         //0.3

    cout << "initialize particlePool..." << endl;
    {
    particlePool tempParticlePool;
    tempParticlePool = myWorldstate.getParticlePool();
    tempParticlePool.initialize(40);
    myWorldstate.setParticlePool(tempParticlePool);
    }

    cout << "initializing particles..." << endl;

    particle tempParticle;
    tempParticle.setMass(0.25);

    tempParticle.setPosition(vectorType(8, 4));
    tempParticle.setVelocity(vectorType(0, 0));
    particle1 = myWorldstate.addParticle(tempParticle);


    tempParticle.setMass(18); //weight 1
    tempParticle.setPosition(vectorType(4.25, 5.5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle2 = myWorldstate.addParticle(tempParticle);

    tempParticle.setMass(0.25);

    tempParticle.setPosition(vectorType(2, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle3 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(3, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle4 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(4, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle5 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(5, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle6 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(6, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle7 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(7, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle8 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(8, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle9 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(9, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle10 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(10, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle11 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, 5));
    tempParticle.setVelocity(vectorType(0, 0));
    particle12 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, 4));
    tempParticle.setVelocity(vectorType(0, 0));
    particle13 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, 3));
    tempParticle.setVelocity(vectorType(0, 0));
    particle14 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, 2));
    tempParticle.setVelocity(vectorType(0, 0));
    particle15 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, 1));
    tempParticle.setVelocity(vectorType(0, 0));
    particle16 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, 0));
    tempParticle.setVelocity(vectorType(0, 0));
    particle17 = myWorldstate.addParticle(tempParticle);

    tempParticle.setPosition(vectorType(11, -1));
    tempParticle.setVelocity(vectorType(0, 0));
    particle18 = myWorldstate.addParticle(tempParticle);


    tempParticle.setMass(10);   //weight 2
    tempParticle.setPosition(vectorType(11, 0));
    tempParticle.setVelocity(vectorType(0, 0));
    particle19 = myWorldstate.addParticle(tempParticle);


    constraint* myConstraint;

    myConstraint = new positionconstraint(particle3, vectorType(2, 5));
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new positionconstraint(particle1, vectorType(8, 4));
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;


    int distanceConstraintType = 1;

    myConstraint = new distanceconstraint(particle3, particle4, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle4, particle5, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle5, particle6, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle6, particle7, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle7, particle8, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle8, particle9, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle9, particle10, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle10, particle11, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle11, particle12, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle12, particle13, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle13, particle14, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle14, particle15, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle15, particle16, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle16, particle17, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle17, particle18, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle18, particle19, 1, distanceConstraintType);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;


    float radius = 1;

    myConstraint = new distanceconstraint(particle3, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle4, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle5, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle6, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle7, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle8, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle9, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle10, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle11, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle12, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle13, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle14, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle15, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle16, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle17, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle18, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle19, particle1, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;


    float otherRadius = 1;

    myConstraint = new distanceconstraint(particle3, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle4, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle5, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle6, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle7, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle8, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle9, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle10, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle11, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle12, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle13, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle14, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle15, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle16, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle17, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle18, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;

    myConstraint = new distanceconstraint(particle19, particle2, radius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;


    myConstraint = new distanceconstraint(particle1, particle2, radius + otherRadius, 2);
    myWorldstate.addConstraint(myConstraint);
    myConstraint = nullptr;


    softforce* mySoftforce;

    mySoftforce = new gravity(9.81, vectorType(0, -1));
    myWorldstate.addSoftforce(mySoftforce);
    mySoftforce = nullptr;

    mySimulator.relaxConstraints(&myWorldstate, 500);
}

void update() {
    //updating of time values and clearing the screen is already done.

    cout << "deltaTime: " << deltaTime << "   FPS: " << 1 / deltaTime << endl;

    if (!paused) {
        cout << "simulating..." << endl;

        if (deltaTime > 1.0f / 45.0f) {
            deltaTime = 1.0f / 45.0f;
        }

        double simulationTime = glfwGetTime();

        //mySimulator.simulate(&myWorldstate, (float)deltaTime * timeScale);
        mySimulator.simulate(&myWorldstate, timeScale / 60.0f);

        simulationTime = glfwGetTime() - simulationTime;

        cout << "simulationTime: " << simulationTime << "simulations/second: " << 1 / simulationTime << endl;
    }

    cout << "updating object positions..." << endl;
    setInstanceList(myWorldstate);

    // Render
    cout << "rendering..." << endl;
    moveCamera();

    camera = projection * view;
    displayInstanceList();
}

// The MAIN function, from here we start our application and run our Program/Game loop
int main() {
    cout << "initializing graphics..." << endl;
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    #if fullscreen
    window = glfwCreateWindow(1366, 768, "Position based physics!", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
    #else
    window = glfwCreateWindow(windowWidth, windowHeight, "Position based physics!", nullptr, nullptr); // Windowed
    #endif // fullscreen

    glfwMakeContextCurrent(window);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Set the required callback functions
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, windowWidth, windowHeight);

    glClearColor(0.4f, 0.1f, 0.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    // Accept fragment if it closer to the camera than the former one
    glClearDepth(100.0f);           //zorg dat deze NIET lager is dan de far clipping plane


    projection = glm::perspective(70.0f, windowWidth / (float)windowHeight, 0.1f, 100.0f);

    cout << "loading world..." << endl;
    loadWorld();

    beginTime = glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    cout << endl << "running..." << endl;

    // Game loop
    while(!glfwWindowShouldClose(window))     {
        // Check and call events
        glfwPollEvents();

        cout << "updating time..." << endl;
        lastTime = currentTime;
        currentTime = glfwGetTime() - beginTime;
        deltaTime = currentTime - lastTime;

        cout << "clearing buffers..." << endl;
        // Clear the buffers
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Update
        cout << "'update'" << endl;
        update();

        cout << endl;
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    cout << endl;

    deleteTriangle();
    deleteSphere();

    glfwTerminate();
    return 0;
}

