#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP_point;

void main(){

    gl_Position =  MVP_point * vec4(vertexPosition_modelspace,1);

}