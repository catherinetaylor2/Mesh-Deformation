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
#include "readObj.hpp"


int main(){

    ObjFile mesh("2D-mesh.obj");
    float* V = mesh.get_vertices();
    float* N = mesh.get_normals();
    int* FV = mesh.get_faceV();
    int* FN = mesh.get_faceN();
    int F = mesh.get_number_of_faces();

    std::cout<<"F "<<F<<"\n";


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
    
//   float vertices[] = {
//      0.0f,  0.5f, 0.0f, // Vertex 1 (X, Y)
//      0.5f, -0.5f, 0.0f, // Vertex 2 (X, Y)
//     -0.5f, -0.5f, 0.0f  // Vertex 3 (X, Y)
// };

float vertices [9*4];

// int c1 = FV[0]-1, c2 = FV[1]-1, c3 = FV[2]-1;
// int i=0;
//  vertices[i] = V[3*c1];
//      vertices[i+2] = V[3*c1+1];
//      vertices[i+1] = V[3*c1+2];
//      vertices[i+3] = V[3*c2];
//      vertices[i+5] = V[3*c2+1];
//      vertices[i+4] = V[3*c2+2];
//      vertices[i+6] = V[3*c3];
//      vertices[i+8] = V[3*c3+1];
//      vertices[i+7] = V[3*c3+2];
int k=0;
for (int i=0; i<9*F; i+=9){
    int c1 = FV[k]-1, c2 = FV[k+1]-1, c3 = FV[k+2]-1;
     vertices[i] = V[3*c1];
     vertices[i+2] = V[3*c1+1];
     vertices[i+1] = V[3*c1+2];
     vertices[i+3] = V[3*c2];
     vertices[i+5] = V[3*c2+1];
     vertices[i+4] = V[3*c2+2];
     vertices[i+6] = V[3*c3];
     vertices[i+8] = V[3*c3+1];
     vertices[i+7] = V[3*c3+2];

    k=k+3;
  

}
for (int i=0; i<9*F;i++){
    std::cout<<"Vi "<<vertices[i]<<" ";
}

GLuint vertexBuffer;
glGenBuffers(1, &vertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
//glBufferData(GL_ARRAY_BUFFER, 9, vertices, GL_STATIC_DRAW);
std::cout<<sizeof(vertices)<<"\n";
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
								0//offset, so where the data starts
		);

for(int i=0; i<F; i++){
          glDrawArrays(GL_LINE_LOOP, 3*i, 3); // 3 indices starting at 0 -> 1 triangle
}
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