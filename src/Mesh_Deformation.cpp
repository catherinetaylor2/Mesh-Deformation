//---------------------------COMPUTER ANIMATION AND GAMES 2: COURSE WORK 1--------------------------------------------------
//Mesh Deformation
//
//Created by Catherine Taylor 24/02/17

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdio>
 #include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "shader.hpp"
#include <eigen3/Eigen/Dense>
#include "readObj.hpp"


#define infinity FLT_MAX

struct Point { //create sructure for selected points.
	GLfloat x;
	GLfloat y;
};

Point p, p_goal, p_previous;
int mouse_clicked = 0;
int number_of_clicks = 0;
int right_click = 0;
GLfloat mouse_world [6] = {0,0,0,0,0,0};
GLint vertex_pos[3]={0,0,0};
int mesh_reset = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){ // if mouse clicked then carries out appropriate action.
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

void mouse_to_world(double mouse_x, double mouse_y, int width, int height, glm::mat4 invProj){ // mouse coords->screen coords-> world coords
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
void find_closest_vertex(float x_coord, float y_coord, float * vertices, int numberOfVertices, float scale){
    float min_distance = infinity, d= infinity;
    int min_index = 0;
    for(int i=0; i<numberOfVertices; i++){
        float x = vertices[3*i], y = vertices[3*i+1];
        d = sqrt((x_coord/scale - x)*(x_coord/scale - x)+(y_coord/scale - y)*(y_coord/scale -y));
        if (d<min_distance){
            min_distance = d;
            min_index = 3*i;
        }
    }
    p.x = vertices[min_index];
    p.y = vertices[min_index+1];
    vertex_pos[number_of_clicks-1]= min_index/3;
    }
void reset(void){ 
    p.x=0;
    p.y=0;
    p_goal.x=0;
    p_goal.y=0;
    p_previous.x=0;
    p_previous.y=0;
    mouse_clicked = 0;
    number_of_clicks = 0;
    right_click = 0;
    for (int i=0; i<6; i++){
        mouse_world[i]=0;
    }
    for (int i=0; i<3; i++){
        vertex_pos[i]=0;
    }
    mesh_reset = 1; 
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if( key== GLFW_KEY_ENTER && action == GLFW_PRESS){
        reset();
    }
}

int main(int argc, char *argv[]){
 
  //  ObjFile mesh("../mesh/dino.obj"); // load mesh information from object file.
	float* V , *N, *VT;
    int *FV, *FN, *F_VT;
 //   mesh.get_vertices(&V);
   // mesh.get_texture(&VT);
    //mesh.get_normals(&N);
    //mesh.get_face_data(&FV, &FN, &F_VT);
    int numberOfFaces, numberOfVertices;
    //int numberOfFaces = mesh.get_numberOfFaces();
    //int numberOfVertices = mesh.get_numberOfVertices();

    ObjFile DinoMesh("../mesh/dino.obj");
    if(!DinoMesh.doesExist()){
        std::cerr<<"Error: Object does not exist \n";
        return false;
    }
	DinoMesh.getMeshData(DinoMesh, &FV, &FN, &F_VT, &VT, &N, &V, &numberOfFaces, &numberOfVertices);


    float scale = 0.2;
    
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int width =1280, height = 720;
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);

    glm::mat4 ModelMatrix = glm::mat4(scale); //Create MVP matrices.
    ModelMatrix[3].w = 1.0;
    glm::mat4 ViewMatrix = glm::lookAt(
        glm::vec3(0,0,-4), // position of camera
        glm::vec3(0,0,1),  // look at vector
        glm::vec3(0,-1,0)  //look up vector
    );
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians (90.0f),         //FOV
        (float)width/(float)height, // Aspect Ratio. 
        0.1f,        // Near clipping plane. 
        100.0f       // Far clipping plane.
    );
    glm::mat4 MVP = projectionMatrix*ViewMatrix*ModelMatrix;
    glm::mat4 ModelMatrix_point = glm::mat4(scale);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set background colour to white.
    glfwSetMouseButtonCallback(window, mouse_button_callback); 
    glfwSetKeyCallback(window, key_callback);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //load shaders for mesh and selected points.
    GLuint programID = LoadShaders( "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl" );
    GLint Mvp = glGetUniformLocation(programID, "MVP");
    GLuint PointprogramID = LoadShaders( "shaders/point_vertexShader.glsl", "shaders/point_fragmentShader.glsl");
    GLint pointMVP = glGetUniformLocation(PointprogramID, "MVP_point");
    GLuint GoalPointprogramID = LoadShaders( "shaders/goal_v.glsl", "shaders/goal.glsl");
    GLint GoalpointMVP = glGetUniformLocation(GoalPointprogramID, "MVP_point");
    GLuint handle = LoadShaders( "shaders/point_vertexShader.glsl", "shaders/point_fragmentShader.glsl");
    GLint handleMVP = glGetUniformLocation(GoalPointprogramID, "MVP_point");
    
    float* vertices = new float[3*numberOfVertices]; // create array of vertices.
    for(int i=0; i<3*numberOfVertices;i+=3){
        vertices[i+1]=V[i];
        vertices[i]=-V[i+2];
        vertices[i+2]=V[i+1];
    }

    unsigned int* indices = new unsigned int [3*numberOfFaces]; // create array containing position of vertices.
    for(int i=0; i<3*numberOfFaces; i+=3){
        indices[i]=FV[i]-1;
        indices[i+1]=FV[i+1]-1;
        indices[i+2]=FV[i+2]-1;   
    }

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*numberOfVertices*sizeof(float),  &vertices[0], GL_DYNAMIC_DRAW);

    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*numberOfFaces*sizeof(unsigned int), indices, GL_DYNAMIC_DRAW); 

    float selected_point [] = {
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f
    };

    GLuint PointBuffer;
    glGenBuffers(1, &PointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, PointBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(selected_point), &selected_point[0], GL_DYNAMIC_DRAW);

    float handle_point[] = {
     1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    GLuint handleBuffer;
    glGenBuffers(1, &handleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, handleBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(handle_point), &handle_point[0], GL_DYNAMIC_DRAW);


    float goal_point[3] = {
        1.0f,1.0f, 0.0f
    };
  
    GLuint GoalPointBuffer;
    glGenBuffers(1, &GoalPointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, GoalPointBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(goal_point), &goal_point[0], GL_DYNAMIC_DRAW);
//-------------------------------------------------------------------------------------------------------------------------------------------
//ALGORITHM - calculations outwith draw loop to speed up.

    int w=1000;
    Eigen::MatrixXf b1(6*numberOfFaces+6,1), G(8,4), G_no_vr(6,4), A1(6*numberOfFaces+6, 2*numberOfVertices), edges(2,8), vertex_new(2*numberOfVertices, 1), A2(3*numberOfFaces+3, numberOfVertices), b2x (3*numberOfFaces + 3,1), b2y(3*numberOfFaces + 3,1), V2x(numberOfVertices,1), V2y(numberOfVertices,1);
     float* V2 = new float[3*numberOfVertices];

     for(int i=0; i<3*numberOfVertices; i++){
         V2[i] = vertices[i];
     }

    for(int i=0; i<6*numberOfFaces+6; i++){
        b1(i,0)=0;
        for (int j=0; j<2*numberOfVertices;j++){
            vertex_new(j,0)= 0;
        }
    }
    for (int i =0; i<3*numberOfFaces+3; i++){
        b2x(i,0)=0;
        b2y(i,0)=0;
        for (int j=0; j<numberOfVertices; j++){
            V2x(j,0)=0;
            V2y(j,0)=0;
        }
    }
    edges<< -1,0,1,0,0,0,0,0,
            0,-1,0,1,0,0,0,0;
    
    for(int j=0; j<8; j++){
        G(j,2) = (j+1)%2;
        G(j,3) = j%2;        
    }
     for(int j=0; j<6; j++){
        G_no_vr(j,2) = (j+1)%2;
        G_no_vr(j,3) = j%2;        
    }

//---------------------------------------------------------------------------------------------------------------------------------------------

  do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        // Use  shader
        glUseProgram(programID);
        glUniformMatrix4fv(Mvp, 1, GL_FALSE, &MVP[0][0]);

        //  1rst attribute buffer : vertices. Including elements buffer with vertex positions.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //draws the triangles correctly as lines.
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        GLint posAttrib = glGetAttribLocation(programID, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(0, // 0  is vertex
                             3, //size of information
                             GL_FLOAT, // type of the data
                             GL_FALSE, // normalised?
                             0, // stride
                             0 // offset
        );        
        glDrawElements(GL_TRIANGLES, 3*numberOfFaces,  GL_UNSIGNED_INT,0); // draw mesh
        
        glUseProgram(PointprogramID);
        glBindBuffer(GL_ARRAY_BUFFER, PointBuffer);
        GLint posAttrib_p = glGetAttribLocation(PointprogramID, "position");
        glEnableVertexAttribArray(posAttrib_p);
   
        glVertexAttribPointer(0, 
                              3, 
                              GL_FLOAT, 
                              GL_FALSE, 
                              6*sizeof(float),
                              0
        );  
        
        GLint colAttrib = glGetAttribLocation(PointprogramID, "Colour");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                       6*sizeof(float), (void*)(3*sizeof(float)));

        if (mouse_clicked){ // react to user inputs and find handle and constraint positions.
            mouse_clicked = !mouse_clicked;
            mouse_to_world(p.x, p.y, width, height, inverseProj);
            find_closest_vertex(p.x, p.y, V2, numberOfVertices, scale);
            mouse_world[2*(number_of_clicks-1)] = p.x;
            mouse_world[2*(number_of_clicks-1)+1] = p.y;  
                 
        }
        if (right_click==1){ // find goal postion
            mouse_to_world_goal(p_goal.x, p_goal.y, width, height, inverseProj); 
            right_click = 0;
        }
     

        for (int i=1; i<3; i++){
            ModelMatrix_point[0].x = scale*mouse_world[2*i];
            ModelMatrix_point[1].y = scale*mouse_world[2*i+1];
            MVP_point = projectionMatrix*ViewMatrix*ModelMatrix_point;
            glUniformMatrix4fv(pointMVP, 1, GL_FALSE, &MVP_point[0][0]);      
            glPointSize(10.0f);
            glDrawArrays(GL_POINTS, 0,1); // draw selected points
        }

        glUseProgram(handle);
        glBindBuffer(GL_ARRAY_BUFFER, handleBuffer);
        GLint posAttrib_h = glGetAttribLocation(handle, "position");
        glEnableVertexAttribArray(posAttrib_h);
   
        glVertexAttribPointer(0, //0 is a magic number which tells opengl what type of information it is, 0 =vertex
                              3, //size of vertex information
                              GL_FLOAT, //type of the data
                              GL_FALSE, //data is normalised
                              6*sizeof(float), //stride
                              0//offset, so where the data starts
        );  
        
        GLint colAttrib_h = glGetAttribLocation(handle, "Colour");
        glEnableVertexAttribArray(colAttrib_h);
        glVertexAttribPointer(colAttrib_h, 
                                3,
                                 GL_FLOAT,
                                 GL_FALSE,
                                6*sizeof(float), 
                                (void*)(3*sizeof(float))
                            );
        ModelMatrix_point[0].x = scale*mouse_world[0];
        ModelMatrix_point[1].y = scale*mouse_world[1];
        MVP_point = projectionMatrix*ViewMatrix*ModelMatrix_point;
        glUniformMatrix4fv(pointMVP, 1, GL_FALSE, &MVP_point[0][0]);      
        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0,1);

       
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

    if (number_of_clicks ==3){ // reset number of clicks.
        number_of_clicks = 0;
    }

//--------------------------------------------------------------------- ------------------------------------------
//ALGORITHM:

        if (mesh_reset){ //MESH RESET FN
            mesh_reset = !mesh_reset; 
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, 3*numberOfVertices*sizeof(float),  &vertices[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            for(int i=0; i<3*numberOfVertices; i++){
                V2[i] = vertices[i];
            }
        }

        if ((p_goal.x !=p_previous.x)&&(p_goal.y!=p_previous.y)&&(!mesh_reset)){ // if goal position changed then carry out algorithm and update mesh.
            p_previous.x=p_goal.x;
            p_previous.y=p_goal.y;
            for(int i=0; i<6*numberOfFaces+6; i++){
                for (int j=0; j<2*numberOfVertices;j++){
                    A1(i,j)=0;
                }
            }
            for (int i =0; i<3*numberOfFaces+3; i++){
                for (int j=0; j<numberOfVertices; j++){
                  A2(i,j)=0;
                }
            }
            
            int vi, vj, vl, vr = 1000;
            float ex, ey;
            Eigen::MatrixXf E(2,2), H,  v(8,1), v_no_vr(6,1), t, T(2,2), b, vr_exist(3*numberOfFaces,1);
             
            b1(6*numberOfFaces) = w*p_goal.x/scale;
            b1(6*numberOfFaces+1) = w*p_goal.y/scale;
            b2x(3*numberOfFaces) = w*p_goal.x/scale; // divide by scale
            b2y(3*numberOfFaces) = w*p_goal.y/scale;

            for (int i=0; i<3; i++){
                A1(6*numberOfFaces + 2*i, 2*vertex_pos[i]) = w;
                A1(6*numberOfFaces + 2*i+1, 2*vertex_pos[i]+1)=w;
                A2(3*numberOfFaces+i,vertex_pos[i] )=w;
            }
            for (int i=1; i<5;i++){
            b1(6*numberOfFaces+1+i) = w*mouse_world[i+1];
            }
            for (int i=1; i<3; i++){
                b2x(3*numberOfFaces+i) = w*mouse_world[2*i];
                b2y(3*numberOfFaces+i) = w*mouse_world[2*i+1];
            }
            for(int i =0; i<numberOfFaces; i++){
                A2(3*i, FV[3*i]-1) = -1;
                A2(3*i, FV[3*i+1]-1)=1;
                A2(3*i +1 , FV[3*i+1]-1) = -1;
                A2(3*i + 1, FV[3*i+2]-1)=1;
                A2(3*i+2, FV[3*i+2]-1) = -1;
                A2(3*i + 2, FV[3*i]-1)=1;

                for(int j=0; j<3;j++){
                    vi = FV[3*i + j]-1;
                    vj = FV[3*i + (j+1)%3]-1;
                    vl = FV[3*i + (j+2)%3]-1;
                    ex = vertices[3*vj ] - vertices[3*vi];
                    ey = vertices[3*vj+1] - vertices[3*vi+1];
                    E(0,0)=ex;
                    E(0,1)= ey;
                    E(1,0) = ey;
                    E(1,1)=-ex;

                    for (int k=0; k<numberOfFaces; k++){
                        if (((vi == FV[3*k]-1)|(vi == FV[3*k+1]-1)|(vi==FV[3*k+2]-1))&((vj == FV[3*k]-1)|(vj == FV[3*k+1]-1)|(vj == FV[3*k+2]-1))&(vl !=FV[3*k]-1)&(vl!=FV[3*k+1]-1)&(vl!=FV[3*k+2]-1)){
                            vr = (vi!=FV[3*k]-1)*(vj!=FV[3*k]-1)*FV[3*k] + (vi!=FV[3*k + 1]-1)*(vj!=FV[3*k+1]-1)*FV[3*k+1]+(vi!=FV[3*k + 2]-1)*(vj!=FV[3*k+2]-1)*FV[3*k+2]-1;
                        }
                    } 
                    vr_exist(3*i + j, 0)=vr;
                
                    if (vr<1000){
                 
                        G(0,0) = vertices[3*vi]; 
                        G(0,1) = vertices[3*vi+1];
                        G(1,0) = vertices[3*vi+1];
                        G(1,1) = -vertices[3*vi];
                        G(2,0) = vertices[3*vj]; 
                        G(2,1) = vertices[3*vj+1];
                        G(3,0) = vertices[3*vj+1];
                        G(3,1) = -vertices[3*vj];
                        G(4,0) = vertices[3*vl]; 
                        G(4,1) = vertices[3*vl+1];
                        G(5,0) = vertices[3*vl+1];
                        G(5,1) = -vertices[3*vl];
                        G(6,0) = vertices[3*vr]; 
                        G(6,1) = vertices[3*vr+1]; 
                        G(7,0) = vertices[3*vr+1];
                        G(7,1) = -vertices[3*vr];                        
                       
                        H = edges - E*((((G.transpose()*G).inverse())*G.transpose()).block<2,8>(0,0));
                
                        A1(6*i+2*j,2*vr) = H(0,6);
                        A1(6*i+2*j+1,2*vr) = H(1,6);
                        A1(6*i+2*j,2*vr+1) = H(0,7);
                        A1(6*i+2*j+1,2*vr+1) = H(1,7);  

                        vr =1000;
                    }
                    else{              

                        G_no_vr(0,0) = vertices[3*vi]; 
                        G_no_vr(0,1) = vertices[3*vi+1];
                        G_no_vr(1,0) = vertices[3*vi+1];
                        G_no_vr(1,1) = -vertices[3*vi];
                        G_no_vr(2,0) = vertices[3*vj]; 
                        G_no_vr(2,1) = vertices[3*vj+1];
                        G_no_vr(3,0) = vertices[3*vj+1];
                        G_no_vr(3,1) = -vertices[3*vj];
                        G_no_vr(4,0) = vertices[3*vl]; 
                        G_no_vr(4,1) = vertices[3*vl+1];
                        G_no_vr(5,0) = vertices[3*vl+1];
                        G_no_vr(5,1) = -vertices[3*vl];

                        H = edges.block<2,6>(0,0) - E*((((G_no_vr.transpose()*G_no_vr).inverse())*G_no_vr.transpose()).block<2,6>(0,0));
                    }
                        A1(6*i+2*j,2*vi) = H(0,0);
                        A1(6*i+2*j+1,2*vi) = H(1,0);
                        A1(6*i+2*j,2*vi+1) = H(0,1);
                        A1(6*i+2*j+1,2*vi+1) = H(1,1);  

                        A1(6*i+2*j,2*vj) = H(0,2);
                        A1(6*i+2*j+1,2*vj) = H(1,2);
                        A1(6*i+2*j,2*vj+1) = H(0,3);
                        A1(6*i+2*j+1,2*vj+1) = H(1,3); 

                        A1(6*i+2*j,2*vl) = H(0,4);
                        A1(6*i+2*j+1,2*vl) = H(1,4);
                        A1(6*i+2*j,2*vl+1) = H(0,5);
                        A1(6*i+2*j+1,2*vl+1) = H(1,5);        
                }
            }
            vertex_new = (A1.transpose()*A1).llt().solve(A1.transpose()*b1);

            for(int i =0; i<numberOfFaces; i++){
                for(int j=0; j<3;j++){

                    vi = FV[3*i + j]-1;
                    vj = FV[3*i + (j+1)%3]-1;
                    vl = FV[3*i + (j+2)%3]-1;
                    vr = vr_exist(3*i +j);
                    ex = vertices[3*vj ] - vertices[3*vi];
                    ey = vertices[3*vj+1] - vertices[3*vi+1];
                    E(0,0)=ex;
                    E(1,0)= ey;
                    E(0,1) = ey;
                    E(1,1)=-ex;            
                                       
              
                    if (vr<1000){  

                        G(0,0) = vertices[3*vi]; 
                        G(0,1) = vertices[3*vi+1];
                        G(1,0) = vertices[3*vi+1];
                        G(1,1) = -vertices[3*vi];
                        G(2,0) = vertices[3*vj]; 
                        G(2,1) = vertices[3*vj+1];
                        G(3,0) = vertices[3*vj+1];
                        G(3,1) = -vertices[3*vj];
                        G(4,0) = vertices[3*vl]; 
                        G(4,1) = vertices[3*vl+1];
                        G(5,0) = vertices[3*vl+1];
                        G(5,1) = -vertices[3*vl];
                        G(6,0) = vertices[3*vr]; 
                        G(6,1) = vertices[3*vr+1];
                        G(7,0) = vertices[3*vr+1];
                        G(7,1) = -vertices[3*vr];  
                    
                        v<<vertex_new(2*vi), vertex_new(2*vi+1), vertex_new(2*vj), vertex_new(2*vj+1), vertex_new(2*vl), vertex_new(2*vl+1), vertex_new(2*vr), vertex_new(2*vr+1) ; 
                        t = ((((G.transpose()*G).inverse())*(G.transpose())).block<2,8>(0,0))*v;
                            
                        vr =1000;                
                    }
                    else{
                                                      
                        G_no_vr(0,0) = vertices[3*vi]; 
                        G_no_vr(0,1) = vertices[3*vi+1];
                        G_no_vr(1,0) = vertices[3*vi+1];
                        G_no_vr(1,1) = -vertices[3*vi];
                        G_no_vr(2,0) = vertices[3*vj]; 
                        G_no_vr(2,1) = vertices[3*vj+1];
                        G_no_vr(3,0) = vertices[3*vj+1];
                        G_no_vr(3,1) = -vertices[3*vj];
                        G_no_vr(4,0) = vertices[3*vl]; 
                        G_no_vr(4,1) = vertices[3*vl+1];
                        G_no_vr(5,0) = vertices[3*vl+1];
                        G_no_vr(5,1) = -vertices[3*vl];

                        v_no_vr<<vertex_new(2*vi), vertex_new(2*vi+1), vertex_new(2*vj), vertex_new(2*vj+1), vertex_new(2*vl), vertex_new(2*vl+1); 
                        t = ((((G_no_vr.transpose()*G_no_vr).inverse())*(G_no_vr.transpose())).block<2,6>(0,0))*v_no_vr;
                    }
                    T<< t(0,0), t(1,0),
                    -t(1,0), t(0,0);
                    b = 1/(sqrt(t(0)*t(0)+t(1)*t(1)))*T*E;            
                    b2x(3*i +j) = b(0,0); 
                    b2y(3*i +j) = b(1,0);
                }
            }
            V2x = (A2.transpose()*A2).llt().solve(A2.transpose()*b2x);
            V2y = (A2.transpose()*A2).llt().solve(A2.transpose()*b2y);

            for (int i=0; i<numberOfVertices; i++){
                V2[3*i] = V2x(i,0);
                V2[3*i + 1]= V2y(i,0);
                V2[3*i + 2] = 0;
            }

            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0,  3*numberOfVertices*sizeof(float), &V2[0]);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    // std::ofstream myfile; //CODE FOR SAVING NEW VERTEX POS FOR KEY FRAMES.
    // myfile.open("new_vertex_positions.txt");
    // for (int i = 0; i<numberOfVertices; i++){
    //     myfile<< "v"<<" "<<V2[3*i]<<" "<<V2[3*i+1]<<" "<<V2[3*i+2]<<"\n";
    // }
    // myfile.close();
//----------------------------------------------------------------------------------------------------------------

        glDisableVertexAttribArray(0);
         glDisableVertexAttribArray(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && glfwWindowShouldClose(window)==0); 

    //Delete to prevent memory leaks.
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &PointBuffer);
    glDeleteBuffers(1, &GoalPointBuffer);
    glDeleteBuffers(1, &handleBuffer);
    glDeleteBuffers(1, &IBO);    
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glDeleteProgram(PointprogramID);
    glDeleteProgram(GoalPointprogramID);
    glDeleteProgram(handle);
    delete[] V;
    delete[] N;
    delete[] FN;
    delete[] FV;
    delete[] vertices;
    delete[] indices;
    delete[] V2;
    
    glfwTerminate();

    return 0;
}
