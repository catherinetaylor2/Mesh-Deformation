//test to determine crashing error

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

int main() {
    
    //C Style function calls, all have a boolean return value which you have to check every time for failure.
    if(!glfwInit()){
       // std::cerr<<"GLFW failed to initialise\n";
    };
    
    //return a pointer to a type of window (which is some struct I believe)

    
    glfwWindowHint(GLFW_SAMPLES, 4); //Anti-Aliasing x4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//opengl version 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    
    auto window = glfwCreateWindow( 1024, 768, "OpenGL Window", NULL, NULL);
    
    if( window == nullptr ){
       printf("Window failed to create, program will now gracefully exit");
              glfwTerminate();
        return -1;
    }
    
    glewExperimental = true; // Needed for core profile
    glfwMakeContextCurrent(window);
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
       printf("GLEW failed to initialised");
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
   glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //makes the screen blue.
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // // Create and compile our GLSL program from the shaders
     GLuint programID = LoadShaders( "/Users/catta/OneDrive/Documents/OpenGL/SimpleVertexShader.vertexshader", "/Users/catta/OneDrive/Documents/OpenGL/SimpleFragmentShader.fragmentshader" );
    
    
    static const GLfloat g_vertex_buffer_data[ ] = {
        0.0f,  0.5f, 0.0f, // Vertex 1: Red
        0.5f, -0.5f, 0.0f, // Vertex 2: Green
        -0.5f, -0.5f, 0.0f  // Vertex 3: Blue
    };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    do{
        // Clear the screen
       glClear( GL_COLOR_BUFFER_BIT );
        
        // Use our shader
      glUseProgram(programID);
        
     //   1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
       GLint posAttrib = glGetAttribLocation(programID, "position");
       glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                             0, 0);
   
        // Draw the triangle !
        glDrawArrays(GL_LINE_LOOP, 0, 3); // 3 indices starting at 0 -> 1 triangle
        
      glDisableVertexAttribArray(0);
        
    //     // Swap buffers
       glfwSwapBuffers(window);
        glfwPollEvents();
        
   } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
   glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
    
}
