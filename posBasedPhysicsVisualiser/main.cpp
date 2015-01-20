#define GLEW_STATIC

#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>      //filestream: word gebruikt voor het lezen van de shaders
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>  //gebruik om glm sneller te laten compileren
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>     //glm gedeelte over matrix transformaties

#define Fullscreen false    //bepaalt later in de code of fullscreen of windowed mode gebruikt word
#define phi 1.6180339887498948482
#define cube false           //bepaalt of de cube vorm word gebruikt of het bal achtige vormpje

GLFWwindow* window;
int windowWidth, windowHeight;
glm::mat4 Projection;
glm::mat4 View;


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
	} else {
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

glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z) {
    return glm::translate(glm::mat4(), glm::vec3(x,y,z));
}

// convenience function that returns a scaling matrix
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z) {
    return glm::scale(glm::mat4(), glm::vec3(x,y,z));
}

struct ModelAsset {
    GLuint shaders = 0;
    GLuint vbo = 0;
    GLuint vao = 0;
    GLenum drawType = GL_TRIANGLES;
    GLint drawStart = 0;
    GLint drawCount = 0;
};

ModelAsset* sphere = nullptr;

const ModelAsset* getSphereAsset() {
    if (sphere == nullptr) {
        sphere = new ModelAsset;

        sphere->shaders = LoadShaders( "vertexShader.glsl", "fragmentShader.glsl" );
        sphere->drawType = GL_TRIANGLES;
        sphere->drawStart = 0;
        sphere->drawCount = 20 * 3;

        glGenBuffers(1, &(sphere->vbo));
        glGenVertexArrays(1, &(sphere->vao));

        // bind the VAO
        glBindVertexArray(sphere->vao);

        // bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, sphere->vbo);

        // Get a handle for our camera uniform
        GLuint MatrixID = glGetUniformLocation(sphere->shaders, "camera");

        /*
        (0, ±1, ±phi)
        (±1, ±phi, 0)
        (±phi, 0, ±1)
        */
        static const GLfloat g_vertex_buffer_data[] = {
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

        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    }
    return sphere;
}

void deleteSphereAsset() {

	// Cleanup VBO and shader
	glDeleteBuffers(1, &(sphere->vbo));
	glDeleteProgram(sphere->shaders);
	glDeleteVertexArrays(1, &(sphere->vao));

    delete sphere;
    sphere = nullptr;
}

struct ModelInstance {
    ModelAsset* asset = nullptr;
    glm::mat4 transform;
};

//renders a single `ModelInstance`
void renderInstance(const ModelInstance& instance) {
    ModelAsset asset = *(instance.asset);


    //bind the shaders
    glUseProgram(asset.shaders);

    //set the shader uniforms
    asset.shaders->setUniform("camera", gCamera.matrix());
    asset.shaders->setUniform("model", inst.transform);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    //bind VAO and draw
    glBindVertexArray(asset->vao);


    // Draw the triangles !
    glDrawArrays(instance.asset.drawType, instance.asset.drawStart, instance.asset.drawCount);

    //unbind everything
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
}
//http://www.tomdalling.com/blog/modern-opengl/05-model-assets-and-instances/



int main() {
                        //INITIALIZE LIBRARIES, WINDOW & CONTEXT

    // Initialize GLFW
    if (glfwInit()) {
        std::cout << "GLFW was successfully initialized" << std::endl;
    } else {
        std::cout << "GLFW could not be initialized" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if true //again, first code is original, second one is from download
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #else
        glfwWindowHint(GLFW_SAMPLES, 4);
    #endif

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    #if Fullscreen
        window = glfwCreateWindow(1366, 768, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
    #else
        window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
    #endif

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    std::cout << "windowWidth = " << windowWidth << "\nwindowHeight = " << windowHeight << std::endl;

    if( window == NULL ){
        std::cout << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}


    //glEnable(GL_CULL_FACE);   //verteces zijn met de hand geschreven, dus niet altijd goed geroteert...

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	#if true    //ALLWAYS LEAVE THIS ON TRUE (only change if you want to mess with peoples brains)
        glDepthFunc(GL_LESS);    //Accept fragment if it closer to the camera than the former one
        glClearDepth(100.0f);           //zorg dat deze NIET lager is dan de far clipping plane
	#else
        glDepthFunc(GL_GREATER);
        glClearDepth(0.0f);           //zorg dat deze NIET lager is dan de far clipping plane
	#endif



	// Projection matrix : 45° Field of View, display ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(45.0f, windowWidth / (float)windowHeight, 0.1f, 100.0f);



	// Dark background
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);


   #if false //verify correct configuration of glew:
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        std::cout << vertexBuffer << std::endl;
    #endif





    double beginTime = glfwGetTime();
    float time;

                        //MAIN-LOOP

    while(!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update

        time = (float)(glfwGetTime() - beginTime);

        View = glm::lookAt(
            glm::vec3(3 * cos(time), 4 + 4 * sin (time / 2.1), 3 * sin(time)), //location of the camera
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

        //render all instances

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



    deleteSphereAsset();




    glfwTerminate();

    #if false // use to loop at console when not opened in IDE
        std::cin.get();
    #endif

    return 0;
}
