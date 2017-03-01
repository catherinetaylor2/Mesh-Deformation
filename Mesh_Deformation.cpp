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
#include <math.h>
#include <stdio.h>
#include "shader.hpp"
#include "readObj.hpp"

struct Point {

	GLfloat x;

	GLfloat y;

};



Point p;



void DrawPoint(float x_val, float y_val){
     glPointSize(10.0f);
         glBegin(GL_POINTS);      
         glVertex3f(x_val, y_val, 0.0f);
         glEnd();

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT &&  action == GLFW_PRESS)
        std::cout<<"button pressed \n";
             double xpos, ypos;
     glfwGetCursorPos(window, &xpos, &ypos);
     std::cout<<"x "<<xpos<<"\n";
std::cout<< (xpos-400.0f)/400.0f << " " << (ypos-400.0f)/400.0f << "\n";

// glLoadIdentity();
//    glClear(GL_COLOR_BUFFER_BIT);
//          glColor3f(1.0f, 0.0f, 0.0f);
//          glPointSize(10.0f);
//          glBegin(GL_POINTS);
//        //  glVertex3f((xpos-400.0f)/400.0f, (ypos-400.0f)/400.0f, 0.0f);
//          glVertex3f(1, 1, 0.0f);
//          glEnd();
p.x = (xpos-400.0f)/400.0f ;
p.y= (ypos-400.0f)/400.0f ;
}



int main(){

    ObjFile mesh("dino2.obj");
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


    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr);


glViewport(0,0,800,800);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0, 800.0, 0.0, 800.0, 1.0, -1.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

//     DrawPoint(1, 1);

    if(window==nullptr){
        printf("window failed \n");
        glfwTerminate();
        return -1;
    }

    glewExperimental = GL_TRUE;
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  //  GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
 //  glfwSetCursor(window, cursor);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/vertex_shader.vertexshader", "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/fragment_shader.fragmentshader" );

float vertices [9*68];

int k=0; //NOTE THIS LAYOUT IS BECAUSE OF MAYA COORD SYSTEM. set up fpr dino2.obj!
for (int i=0; i<9*F; i+=9){
    int c1 = FV[k]-1, c2 = FV[k+1]-1, c3 = FV[k+2]-1;
    vertices[i+1] = -0.07*V[3*c1];
    vertices[i] = 0.07*V[3*c1+1];
    vertices[i+2] = 0.07*V[3*c1+2];
    vertices[i+4] = -0.07*V[3*c2];
    vertices[i+3] = 0.07*V[3*c2+1];
    vertices[i+5] = 0.07*V[3*c2+2];
    vertices[i+7] = -0.07*V[3*c3];
    vertices[i+6] = 0.07*V[3*c3+1];
    vertices[i+8] = 0.07*V[3*c3+2];
    k=k+3;
}


GLuint vertexBuffer;
glGenBuffers(1, &vertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), &vertices[0], GL_STATIC_DRAW);



glfwSetMouseButtonCallback(window, mouse_button_callback);



do{
       // Clear the screen
       glClear( GL_COLOR_BUFFER_BIT );
       
       // Use our shader
        glUseProgram(programID);

//  glPointSize(10.0f);
//  glColor3f(1.0f, 0.0f, 0.0f);
//          glBegin(GL_POINTS);      
//          glVertex3f(100, 100, 0.0f);
//             glVertex3f(0, 0, 0.0f);
//          glEnd();

      //  1rst attribute buffer : vertices
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
// glPointSize(10.0f);
// glColor3f(1.0f, 0.0f, 0.0f);
// glDrawArrays(GL_POINTS, 0,1);



            glDisableVertexAttribArray(0);

      // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }



 while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );


//Delete to prevent memory leaks.
glDeleteBuffers(1, &vertexBuffer);
glDeleteVertexArrays(1, &VertexArrayID);
glDeleteProgram(programID);
//glfwDestroyCursor(cursor);
delete V;
delete N;
delete FN;
delete FV;


  glfwTerminate();

    return 0;
}
