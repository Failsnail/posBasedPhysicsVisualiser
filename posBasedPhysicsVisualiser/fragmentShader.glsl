#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

out vec3 color;

void main(){
    //color = vec3(1,1,1);
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color = fragmentColor;
}
