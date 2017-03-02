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
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "readObj.hpp"

struct Point {
	GLfloat x;
	GLfloat y;
};

Point p;
int mouse_clicked = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT &&  action == GLFW_PRESS){
        mouse_clicked = 1;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        p.x = xpos;
        p.y= ypos;
    }
}

void mouse_to_world(double mouse_x, double mouse_y, int width, int height, glm::mat4 invProj){


float x = (float)(-2.0f*mouse_x)/width + 1, y = (float)(2.0f*mouse_y)/height - 1;

    glm::vec4 V = glm::vec4(x, y, 0, 1);
    glm::vec4 world_coords = invProj*V;
    p.x = world_coords.x*world_coords.z/world_coords.w;
    p.y = world_coords.y*world_coords.z/world_coords.w;


}


int main(){

    ObjFile mesh("din32.obj");
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

    int width =1280, height = 720;
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);

    glm::mat4 ModelMatrix = glm::mat4(0.35);
    ModelMatrix[3].w = 1.0;
    glm::mat4 ViewMatrix = glm::lookAt(
        glm::vec3(0,0,-4), // the position of your camera, in world space
        glm::vec3(0,0,1),   // where you want to look at, in world space
        glm::vec3(0,-1,0)        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
    );
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians (90.0f),         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        (float)width/(float)height, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f       // Far clipping plane. Keep as little as possible.
    );
    glm::mat4 MVP = projectionMatrix*ViewMatrix*ModelMatrix;

    glm::mat4 ModelMatrix_point = glm::mat4(0.35);
   ModelMatrix_point[3].w = 1.0f;
    glm::mat4 ViewMatrix_point = glm::lookAt(
        glm::vec3(0,0,-4), 
        glm::vec3(0,0,1),   
        glm::vec3(0,-1,0)
    );
    glm::mat4 projectionMatrix_point = glm::perspective(
        glm::radians (90.0f),         
        (float)width/(float)height,
        0.1f,        
        100.0f      
    );
   glm::mat4 MVP_point = projectionMatrix_point*ViewMatrix_point*ModelMatrix_point;


glm::mat4 inverseProj = inverse(projectionMatrix*ViewMatrix);
p.x = 0;
p.y=0;

    if(window==nullptr){
        printf("window failed \n");
        glfwTerminate();
        return -1;
    }

    glewExperimental = GL_TRUE;
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/vertex_shader.vertexshader", "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/fragment_shader.fragmentshader" );
    GLint uniMvp = glGetUniformLocation(programID, "MVP");

     GLuint PointprogramID = LoadShaders( "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/point_vertexShader.vertexshader", "/Users/catta/OneDrive/Documents/Course work/Computer animation and games 2/Mesh-Deformation/point_fragmentShader.fragmentshader");
    GLint pointMVP = glGetUniformLocation(PointprogramID, "MVP_point");



    float vertices [9*68];

    int k=0; //NOTE THIS LAYOUT IS BECAUSE OF MAYA COORD SYSTEM. set up fpr dino2.obj!
    for (int i=0; i<9*F; i+=9){
        int c1 = FV[k]-1, c2 = FV[k+1]-1, c3 = FV[k+2]-1;
        vertices[i+1] = V[3*c1];
        vertices[i] = V[3*c1+1];
        vertices[i+2] = V[3*c1+2];
        vertices[i+4] = V[3*c2];
        vertices[i+3] = V[3*c2+1];
        vertices[i+5] = V[3*c2+2];
        vertices[i+7] = V[3*c3];
        vertices[i+6] = V[3*c3+1];
        vertices[i+8] = V[3*c3+2];
        k=k+3;
    }

    float selected_point [3];
    selected_point[0]=1;
    selected_point[1]=1;
    selected_point[2]=0;


    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);

    GLuint PointBuffer;
    glGenBuffers(1, &PointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, PointBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(selected_point), &selected_point[0], GL_DYNAMIC_DRAW);
  

//do{
    while(!glfwWindowShouldClose(window))
{


       // Clear the screen
       glClear( GL_COLOR_BUFFER_BIT );
       
       // Use our shader
        glUseProgram(programID);
glUniformMatrix4fv(uniMvp, 1, GL_FALSE, &MVP[0][0]);

    
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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

glUseProgram(PointprogramID);
//glUniformMatrix4fv(pointMVP, 1, GL_FALSE, &pointMVP[0][0]);


if (mouse_clicked){
    mouse_clicked = !mouse_clicked;
   mouse_to_world(p.x, p.y, width, height, inverseProj);
}

ModelMatrix_point[0].x = p.x;
ModelMatrix_point[1].y = p.y;
MVP_point = projectionMatrix*ViewMatrix*ModelMatrix_point;
glUniformMatrix4fv(pointMVP, 1, GL_FALSE, &MVP_point[0][0]);
 glBindBuffer(GL_ARRAY_BUFFER, PointBuffer);
        GLint posAttrib_p = glGetAttribLocation(PointprogramID, "position");
        glEnableVertexAttribArray(posAttrib_p);
  glVertexAttribPointer(0, //0 is a magic number which tells opengl what type of information it is, 0 =vertex
            3, //size of vertex information
            GL_FLOAT, //type of the data
            GL_FALSE, //data is normalised
            0, //stride
            0//offset, so where the data starts
        );
    


glPointSize(10.0f);
glDrawArrays(GL_POINTS, 0,1);



            glDisableVertexAttribArray(0);

      // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }



 while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );


//Delete to prevent memory leaks.
glDeleteBuffers(1, &vertexBuffer);
glDeleteBuffers(1, &PointBuffer);
glDeleteVertexArrays(1, &VertexArrayID);
glDeleteProgram(programID);
glDeleteProgram(PointprogramID);
//glfwDestroyCursor(cursor);
delete V;
delete N;
delete FN;
delete FV;


  glfwTerminate();

    return 0;
}
