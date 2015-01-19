#version 330 core

// Interpolated values from the vertex shaders
in float depth;

// Ouput data
out vec3 color;

void main() {

	color = vec3(1,1,1) * (1 - depth / 15 + 0.1 / depth);

}
