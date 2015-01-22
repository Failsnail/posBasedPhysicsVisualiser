#version 330 core

//uniform mat4 MVP;

layout (location = 12) in vec3 position;

void main() {

    //vec4 tempVector = vec4(position, 1.0f);
    //gl_Position = MVP * tempVector;
    gl_Position = vec4(position, 1.0f);
}
