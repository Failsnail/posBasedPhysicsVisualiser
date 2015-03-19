#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 12) in vec3 position;

// Output data ; will be interpolated for each fragment.
out float depth;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(position, 1.0f);
	depth = gl_Position.z;
}
