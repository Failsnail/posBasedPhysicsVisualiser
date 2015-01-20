#version 330 core

//input
uniform mat4 camera;
uniform mat4 model;

//Input vertex data, different for all executions of this shader.
in vec3 vert;

//Output data ; will be interpolated for each fragment.
out float depth;

//Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {

	//Output position of the vertex
	gl_Position = camera * model * vec4(vert, 1);

    depth = gl_Position.z;
}
