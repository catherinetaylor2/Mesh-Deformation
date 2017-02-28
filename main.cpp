//test to determine crashing error
//should not crash right now
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

int main() {
    
 
    if(!glfwInit()){
       // std::cerr<<"GLFW failed to initialise\n"; //CRASHES IF UNCOMMENTED
    };
 

    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    
    auto window = glfwCreateWindow( 1024, 768, "OpenGL Window", NULL, NULL);
    
    if( window == nullptr ){
       printf("Window failed to create, program will now gracefully exit"); //IF STD::CERR THEN IT CRASHES
              glfwTerminate();
        return -1;
    }
    
    glewExperimental = true; 
      if (glewInit() != GLEW_OK) {
       printf("GLEW failed to initialised");
        glfwTerminate();
        return -1;
    }
 
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
   glClearColor(0.0f, 0.0f, 0.4f, 0.0f); 
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    
   //  GLuint programID = LoadShaders( "/Users/catta/OneDrive/Documents/OpenGL/SimpleVertexShader.vertexshader", "/Users/catta/OneDrive/Documents/OpenGL/SimpleFragmentShader.fragmentshader" );
    
    
    static const GLfloat g_vertex_buffer_data[ ] = {
        0.0f,  0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f 
    };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    do{
      
       glClear( GL_COLOR_BUFFER_BIT );
       
      glUseProgram(programID);
        
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
       GLint posAttrib = glGetAttribLocation(programID, "position");
       glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                             0, 0);
   
        
        glDrawArrays(GL_LINE_LOOP, 0, 3); 
        
      glDisableVertexAttribArray(0);
        
          glfwSwapBuffers(window);
        glfwPollEvents();
        
   }     while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
 
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
   glDeleteProgram(programID);
    
       glfwTerminate();
    
    return 0;
    
}
