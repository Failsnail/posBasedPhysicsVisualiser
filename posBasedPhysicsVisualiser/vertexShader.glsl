#version 330 core

// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;
in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
    gl_Position = MVP * v;

    fragmentColor = vertexColor;
}
