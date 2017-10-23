#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 Colour;
out vec3 fragmentColor;
uniform mat4 MVP_point;
in vec3 Colour;

void main(){

    gl_Position =  MVP_point * vec4(vertexPosition_modelspace,1);
    fragmentColor = Colour;
}