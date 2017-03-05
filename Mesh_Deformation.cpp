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
#include "shader.hpp"
#include "readObj.hpp"
#include "matrix.hpp"

#define infinity 100000000

struct Point {
	GLfloat x;
	GLfloat y;
};

Point p, p_goal;
int mouse_clicked = 0;
int number_of_clicks = 0;
int right_click = 0;
GLfloat mouse_world [6] = {0,0,0,0,0,0};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT &&  action == GLFW_PRESS){
        mouse_clicked = 1;
        number_of_clicks++;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        p.x = xpos;
        p.y= ypos;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT &&  action == GLFW_PRESS){
        right_click =1;
       double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        p_goal.x = xpos;
        p_goal.y= ypos;
    }
}

void mouse_to_world(double mouse_x, double mouse_y, int width, int height, glm::mat4 invProj){
    float x = (float)(-2.0f*mouse_x)/(float)width + 1, y = (float)(2.0f*mouse_y)/(float)height - 1;
    glm::vec4 V = glm::vec4(x, y, 0, 1);
    glm::vec4 world_coords = invProj*V;
    p.x = world_coords.x*world_coords.z/world_coords.w;
    p.y = world_coords.y*world_coords.z/world_coords.w;
}
void mouse_to_world_goal(double mouse_x, double mouse_y, int width, int height, glm::mat4 invProj){
    float x = (float)(-2.0f*mouse_x)/(float)width + 1, y = (float)(2.0f*mouse_y)/(float)height - 1;
    glm::vec4 V = glm::vec4(x, y, 0, 1);
    glm::vec4 world_coords = invProj*V;
    p_goal.x = world_coords.x*world_coords.z/world_coords.w;
    p_goal.y = world_coords.y*world_coords.z/world_coords.w;
}
void find_closest_vertex(float x_coord, float y_coord, float * vertices, int number_of_faces){
    float min_distance = infinity, d= infinity;
    int min_index = 0;
    for(int i=0; i<number_of_faces; i++){
        float x = vertices[3*i], y = vertices[3*i+1];
        d = sqrt((x_coord/0.35 - x)*(x_coord/0.35 - x)+(y_coord/0.35 - y)*(y_coord/0.35 -y));
        if (d<min_distance){
            min_distance = d;
            min_index = 3*i;
        }
    }
    p.x = vertices[min_index];
    p.y = vertices[min_index+1];
}

int main(){

    ObjFile mesh("din32.obj");
    float* V = mesh.get_vertices();
    float* N = mesh.get_normals();
    int* FV = mesh.get_faceV();
    int* FN = mesh.get_faceN();
    int F = mesh.get_number_of_faces();

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
        glm::vec3(0,0,1),  // look at vector
        glm::vec3(0,-1,0)  //look up vector
    );
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians (90.0f),         // The horizontal Field of View, in degrees 
        (float)width/(float)height, // Aspect Ratio. 
        0.1f,        // Near clipping plane. 
        100.0f       // Far clipping plane.
    );
    glm::mat4 MVP = projectionMatrix*ViewMatrix*ModelMatrix;
    glm::mat4 ModelMatrix_point = glm::mat4(0.35);
    ModelMatrix_point[3].w = 1.0f;
    glm::mat4 MVP_point = projectionMatrix*ViewMatrix*ModelMatrix_point;
    glm::mat4 inverseProj = inverse(projectionMatrix*ViewMatrix);

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "vertex_shader.vertexshader", "fragment_shader.fragmentshader" );
    GLint Mvp = glGetUniformLocation(programID, "MVP");
    GLuint PointprogramID = LoadShaders( "point_vertexShader.vertexshader", "point_fragmentShader.fragmentshader");
    GLint pointMVP = glGetUniformLocation(PointprogramID, "MVP_point");
    GLuint GoalPointprogramID = LoadShaders( "point_vertexShader.vertexshader", "goal.fragmentshader");
    GLint GoalpointMVP = glGetUniformLocation(GoalPointprogramID, "MVP_point");

     float vertices [3*66];
    for(int i=0; i<3*66;i+=3){
        vertices[i+1]=V[i];
        vertices[i]=V[i+1];
        vertices[i+2]=V[i+2];
    }

    unsigned int indices [3*68] ;

    for(int i=0; i<3*F; i+=3){
        indices[i]=FV[i]-1;
        indices[i+1]=FV[i+1]-1;
        indices[i+2]=FV[i+2]-1;   
    }

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);

    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    float selected_point [9] ={
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    GLuint PointBuffer;
    glGenBuffers(1, &PointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, PointBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(selected_point), &selected_point[0], GL_DYNAMIC_DRAW);

    float goal_point[3] = {
        1.0f,1.0f, 0.0f
    };
  
    GLuint GoalPointBuffer;
    glGenBuffers(1, &GoalPointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, GoalPointBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(goal_point), &goal_point[0], GL_DYNAMIC_DRAW);
//-------------------------------------------------------------------------------------------------------------------------------------------
//ALGORITHM

int w=1000;

float b1[6*68+6], A1[6*68+6][2*66];
for(int i=0; i<6*68; i++){
    b1[i]=0;
    for (int j=0; j<2*66;j++){
        A1[i][j]=0;
    }
}
int edges [2][8] ={ {-1,0,1,0,0,0,0,0},{0,-1,0,1,0,0,0,0}};







//---------------------------------------------------------------------------------------------------------------------------------------------







    while(!glfwWindowShouldClose(window)){

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        // Use our shader
        glUseProgram(programID);
        glUniformMatrix4fv(Mvp, 1, GL_FALSE, &MVP[0][0]);

        //  1rst attribute buffer : vertices. Including elements buffer with vertex positions.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //draws the triangles correctly as lines.
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        GLint posAttrib = glGetAttribLocation(programID, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(0, //0 is a magic number which tells opengl what type of information it is, 0 =vertex
                             3, //size of vertex information
                             GL_FLOAT, //type of the data
                             GL_FALSE, //data is normalised
                             0, //stride
                             0//offset, so where the data starts
        );        
        glDrawElements(GL_TRIANGLES, 3*F,  GL_UNSIGNED_INT,0);
        
        glUseProgram(PointprogramID);
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

        if (mouse_clicked){
            mouse_clicked = !mouse_clicked;
            mouse_to_world(p.x, p.y, width, height, inverseProj);
            find_closest_vertex(p.x, p.y, vertices, F);
            mouse_world[2*(number_of_clicks-1)] = p.x;
            mouse_world[2*(number_of_clicks-1)+1] = p.y;            
        }
        if (right_click==1){
            mouse_to_world_goal(p_goal.x, p_goal.y, width, height, inverseProj); //maybe need to divide by 0.35; 
            right_click = 0;
        }
        for (int i=0; i<3; i++){
            ModelMatrix_point[0].x = 0.35*mouse_world[2*i];
            ModelMatrix_point[1].y = 0.35*mouse_world[2*i+1];
            MVP_point = projectionMatrix*ViewMatrix*ModelMatrix_point;
            glUniformMatrix4fv(pointMVP, 1, GL_FALSE, &MVP_point[0][0]);        
            glPointSize(10.0f);
            glDrawArrays(GL_POINTS, 0,1);
        }
       
        glUseProgram(GoalPointprogramID);
        glBindBuffer(GL_ARRAY_BUFFER, GoalPointBuffer);
        GLint posAttrib_g = glGetAttribLocation(GoalPointprogramID, "position");
        glEnableVertexAttribArray(posAttrib_g); 
        glVertexAttribPointer(0, //0 is a magic number which tells opengl what type of information it is, 0 =vertex
                              3, //size of vertex information
                              GL_FLOAT, //type of the data
                              GL_FALSE, //data is normalised
                              0, //stride
                              0//offset, so where the data starts
        );  
        ModelMatrix_point[0].x = p_goal.x;
        ModelMatrix_point[1].y =p_goal.y;
        MVP_point = projectionMatrix*ViewMatrix*ModelMatrix_point;
        glUniformMatrix4fv(GoalpointMVP, 1, GL_FALSE, &MVP_point[0][0]);

        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0,1);

        if (number_of_clicks==3){
            number_of_clicks = 0;
        }

//---------------------------------------------------------------------------------------------------------------
//ALGORITHM
int vi, vj, vl, vr = 0;
float ex, ey, M[4][4], GT[4][8], GTG_array[4][4], Gk[4][8], H[2][8];
glm::mat4 GTG;
b1[6*F] = w*p_goal.x;
b1[6*F+1] = w*p_goal.y;
for (int i=0; i<4; i++){
    b1[6*F+2+i] = w*mouse_world[i];
}
for(int i =0; i<F; i++){
    for(int j=0; j<3;j++){
        vi = FV[3*i + j];
        vj = FV[3*i + j%2 + 1];
        vl = FV[3*i + 2*(j==0)+(j-1)];
        ex = vertices[3*vj ] - vertices[3*vi];
        ey = vertices[3*vj+1] - vertices[3*vi+1];
        glm::mat2 E;
        E[0][0]=ex;
        E[1][0]= ey;
        E[0][1] = -ey;
        E[1][1]=ex;
        

        for (int k=0; k<F; k++){
            if (((vi == FV[3*k])|(vi == FV[3*k+1])|(vi==FV[3*k+2]))&((vj == FV[3*k])|(vj == FV[3*k+1])|(vj == FV[3*k+2]))&(vl !=FV[3*k])&(vl!=FV[3*k+1])&(vl!=FV[3*k+2])){
                vr = (vi!=FV[3*k])*(vj!=FV[3*k])*FV[3*k] + (vi!=FV[3*k + 1])*(vj!=FV[3*k+1])*FV[3*k+1]+(vi!=FV[3*k + 2])*(vj!=FV[3*k+2])*FV[3*k+2];
             }
            if (vr!=0){
                float G[8][4] = {{vertices[3*vi], vertices[3*vi+1], 1, 0}, {vertices[3*vi + 1], -vertices[3*vi], 0,1},{vertices[3*vj], vertices[3*vj+1], 1, 0}, {vertices[3*vj + 1], -vertices[3*vj], 0,1}, {vertices[3*vl], vertices[3*vl+1], 1, 0}, {vertices[3*vl + 1], -vertices[3*vl], 0,1}, {vertices[3*vr], vertices[3*vr+1], 1, 0}, {vertices[3*vr + 1], -vertices[3*vr], 0,1}};
                matrix_transpose(G, 8, 4, GT);
                matrix_multiply_8(GT, G, 4, 8, 8, 4, M);
                array_to_matrix(GTG, M);
                glm::mat4 GTG_inverse = inverse(GTG);
                matrix_to_array(GTG_inverse, GTG_array);
                matrix_multiply_4(GTG_array, GT, 4,4,4,8,Gk);

            }
            else if(vr==0){
                float G[6][4] = {{vertices[3*vi], vertices[3*vi+1], 1, 0}, {vertices[3*vi + 1], -vertices[3*vi], 0,1},{vertices[3*vj], vertices[3*vj+1], 1, 0}, {vertices[3*vj + 1], -vertices[3*vj], 0,1}, {vertices[3*vl], vertices[3*vl+1], 1, 0}, {vertices[3*vl + 1], -vertices[3*vl], 0,1}};

            }
        }
    }
}



//----------------------------------------------------------------------------------------------------------------

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
    glDeleteBuffers(1, &GoalPointBuffer);
    glDeleteBuffers(1, &IBO);    
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glDeleteProgram(PointprogramID);
    glDeleteProgram(GoalPointprogramID);
    delete V;
    delete N;
    delete FN;
    delete FV;

    glfwTerminate();

    return 0;
}
