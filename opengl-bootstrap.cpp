// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#ifdef __APPLE_CC__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#else 
#include <GL/glew.h>
#endif

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <iostream>

#include <common/Init.h>
#include <common/Shader.h>

using namespace glm;
using namespace std; 


float euler(float stepSize, float lastValue, float yprim)
{
    return lastValue + stepSize*yprim;
}



int main( void )
{
  
  //Create init object
  Init init = Init();

  //Initialize glfw
  init.glfw(3,3);

  //Open a window
  GLFWwindow* window = init.window();

  //Print window info
  init.printWindowInfo(window);

  //Make opened window current context
  glfwMakeContextCurrent(window);

  
  init.glew();

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  //Load in shaders
  static ShaderProgram prog("../vertShader.vert", "../fragShader.frag");

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);


//the weighttt
  GLfloat ballz[] = {

    0.0f, 0.0f, 0.0f,
  };

//wallz
float length = 1.0f;
float height = 0.7f;
float width = 0.1f;
static const GLfloat walls[] = {

//West wall
-length, height, 0.0f,
-length + width, height, 0.0f,
-length + width, -height, 0.0f,

-length + width, -height, 0.0f,
-length, -height, 0.0f,
-length, height, 0.0f,

//South wall
-length, -height, 0.0f,
length, -height, 0.0f,
length, -height - width, 0.0f,

length, -height - width, 0.0f,
-length, -height - width, 0.0f,
-length, -height, 0.0f,

//East wall
length - width, height, 0.0f,
length, height, 0.0f,
length, -height, 0.0f,

length, -height, 0.0f,
length - width, -height, 0.0f,
length - width, height, 0.0f,
};

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(walls), walls, GL_STATIC_DRAW);


  //vertex buffer for the ball
  GLuint ballzbuffer; 
  glGenBuffers(1, &ballzbuffer); 
  glBindBuffer(GL_ARRAY_BUFFER, ballzbuffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(ballz), ballz, GL_STATIC_DRAW); 

  //mathematricks
 /*glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);*/ 

  float pi = 3.14159, ropeLength = 0.5, stepSize = 0.01, g = 9.82;
  float theta = (-pi / 4), velocity = 10, acceleration = 0;
  float xPosition = 0.0f, yPosition = 0.0f;
      
  do{


    


    acceleration = (-g / ropeLength)*sin(theta);
    velocity = euler(stepSize, velocity, acceleration);
    theta = euler(stepSize, theta, velocity);
    xPosition = ropeLength*sin(theta) - 1;
    yPosition = ropeLength*(1 - cos(theta));

    ballz[0] = xPosition; 
    ballz[1] = yPosition;
    ballz[2] = 0.0f; 

    glGenBuffers(1, &ballzbuffer); 
    glBindBuffer(GL_ARRAY_BUFFER, ballzbuffer); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballz), ballz, GL_STATIC_DRAW);
    cout << "x: " << xPosition << endl; 
    cout << "y: " << yPosition << endl; 

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    prog();


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3*6); // 3 indices starting at 0 -> 1 triangle
    //for them baallz

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, ballzbuffer);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    // Draw the triangle !
    glDrawArrays(GL_POINTS, 0, 1); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } // Check if the ESC key was pressed or the window was closed
  while( !glfwWindowShouldClose(window) );

  // Close OpenGL window and terminate GLFW
  glfwDestroyWindow(window);
  glfwTerminate();
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  exit(EXIT_SUCCESS);
}
