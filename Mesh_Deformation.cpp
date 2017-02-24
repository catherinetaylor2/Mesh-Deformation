//Mesh Deformation
//
//Created by Catherine Taylor 24/02/17



#include <glew.h>
#include <glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdio>
#include <iostream>


int main(){

glfwInit();

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

glfwMakeContextCurrent(window);
while(!glfwWindowShouldClose(window)){
    glfwSwapBuffers(window);
    glfwPollEvents();
}

glewExperimental = GL_TRUE;
glewInit();


GLuint vertexBuffer;
glGenBuffers(1, &vertexBuffer);

printf("%u\n", vertexBuffer);


if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  glfwTerminate();

    return 0;
}