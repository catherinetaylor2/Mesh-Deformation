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
#include "shader.hpp"


int main(){

glfwInit();
 
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

auto window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

if(window==nullptr){
    printf("window failed \n");
    glfwTerminate();
    return -1;
}

glewExperimental = GL_TRUE;
glfwMakeContextCurrent(window);
glewInit();

     glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
     
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
GLuint programID = LoadShaders( "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/vertex_shader.vertexshader", "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/fragment_shader.fragmentshader" );
    
  float vertices[] = {
     0.0f,  0.5f, 0.0f, // Vertex 1 (X, Y)
     0.5f, -0.5f, 0.0f, // Vertex 2 (X, Y)
    -0.5f, -0.5f, 0.0f  // Vertex 3 (X, Y)
};



GLuint vertexBuffer;
glGenBuffers(1, &vertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

do{
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Use our shader
        glUseProgram(programID);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        GLint posAttrib = glGetAttribLocation(programID, "position");
        glEnableVertexAttribArray(posAttrib);
       
	    glVertexAttribPointer(0, //0 is a magic number which tells opengl what type of information it is, 0 =vertex
								3, //size of vertex information
								GL_FLOAT, //type of the data
								GL_FALSE, //data is normalised
								0, //stride
								0 //offset, so where the data starts
		);

          glDrawArrays(GL_LINE_LOOP, 0, 3); // 3 indices starting at 0 -> 1 triangle
        
        glDisableVertexAttribArray(0);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }


 while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );

  glfwTerminate();

    return 0;
}