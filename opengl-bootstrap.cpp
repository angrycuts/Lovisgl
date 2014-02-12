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
#include <math.h>
using namespace glm;
using namespace std; 


float euler(float stepSize, float lastValue, float yprim) {
    return lastValue + stepSize*yprim;
}



int main( void ) {
  
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
float wlength = 1.0f;
float wheight = 0.7f;
float wwidth = 0.1f;

static const GLfloat walls[] = {
  //West wall
  -wlength, wheight, 0.0f,
  -wlength + wwidth, wheight, 0.0f,
  -wlength + wwidth, -wheight, 0.0f,

  -wlength + wwidth, -wheight, 0.0f,
  -wlength, -wheight, 0.0f,
  -wlength, wheight, 0.0f,

  //South wall
  -wlength, -wheight, 0.0f,
  wlength, -wheight, 0.0f,
  wlength, -wheight - wwidth, 0.0f,

  wlength, -wheight - wwidth, 0.0f,
  -wlength, -wheight - wwidth, 0.0f,
  -wlength, -wheight, 0.0f,

  //East wall
  wlength - wwidth, wheight, 0.0f,
  wlength, wheight, 0.0f,
  wlength, -wheight, 0.0f,

  wlength, -wheight, 0.0f,
  wlength - wwidth, -wheight, 0.0f,
  wlength - wwidth, wheight, 0.0f,
};

GLfloat ropez[] = {
  0.0f, 0.5f, 0.0f, 
  0.0f, 0.0f, 0.0f,
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

//vertex buffer for the roopez
GLuint ropebuffer; 
glGenBuffers(1, &ropebuffer); 
glBindBuffer(GL_ARRAY_BUFFER, ropebuffer); 
glBufferData(GL_ARRAY_BUFFER, sizeof(ropez), ropez, GL_STATIC_DRAW);  
//mathematricks
/*glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);*/ 

float pi = 3.14159f, ropeLength = 0.5f, stepSize = 0.01f, g = 9.82f;
float theta = (-pi / 4.0f), velocity = 5.0f, acceleration = 0.0f;
float xPosition = 0.0f, yPosition = 0.0f;
float radiusOne = 0.01f, volumeOne = (4.0f*pi*pow(radiusOne,3))/3 ;
float areaOne = 4.0f*pi*pow(radiusOne,2), density = 11340.0f; 
float massOne = density*volumeOne;
float airconstant = 0.47f;
bool isPressed = false;
bool firstCheck = false;
float xAcc = 0.0f, yAcc = 0.0f, xVel = 0.0f, yVel =0.0f;
float xAirres = 0.0f;
float yAirres = 0.0f;

do{
  //If space is pressed; throw ball!
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isPressed == false) {
    isPressed = true;
  }
  //Pendulum
  if (isPressed == false) {
    float airres = (0.5f*pow(velocity,2)*areaOne*airconstant)/massOne;
    
    if (velocity < 0) {
      airres = airres*-1.0f;
    }

    acceleration = (-g / ropeLength)*sin(theta);
    velocity = euler(stepSize, velocity, acceleration) - airres;
    theta = euler(stepSize, theta, velocity);
    xPosition = ropeLength*sin(theta);
    yPosition = ropeLength*(1 - cos(theta));

    ballz[0] = xPosition; 
    ballz[1] = yPosition;

    ropez[3] = ballz[0];
    ropez[4] = ballz[1];

    glGenBuffers(1, &ropebuffer); 
    glBindBuffer(GL_ARRAY_BUFFER, ropebuffer); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(ropez), ropez, GL_STATIC_DRAW);
  }
  //Projectile
  else {
    if (firstCheck == false){
      xAcc = 0.0f; 
      yAcc = acceleration*sin(theta)-g; 
      xVel = velocity*cos(theta); 
      yVel = velocity*sin(theta);
      firstCheck = true;
    }
    xAirres = (0.5f*pow(xVel,2)*areaOne*airconstant)/massOne;
    yAirres = (0.5f*pow(yVel,2)*areaOne*airconstant)/massOne;
    
    if (xVel < 0) {
      xAirres = -1*xAirres;
    }

    if (yVel < 0) {
      yAirres = -1*yAirres;
    }
    cout << xVel << endl;
    xVel = euler(stepSize, xVel, xAcc) - xAirres;
    yVel = euler(stepSize, yVel, yAcc) - yAirres;
    xPosition = euler(stepSize, xPosition, xVel);
    yPosition = euler(stepSize, yPosition, yVel);

    //Hit ground (add radius)
    if (yPosition < -wheight){
      yVel = -0.8*yVel;
      yPosition = -wheight;
      xVel = xVel*0.8f;
    }
    //Hit the west or east wall (add radius)
    if (xPosition < -wlength + wwidth) {
      xVel = -0.8*xVel;
      xPosition = -wlength + wwidth;
    }
    if (xPosition > wlength - wwidth) {
      xVel = -0.8*xVel;
      xPosition = wlength - wwidth;
    }
    ballz[0] = xPosition;
    ballz[1] = yPosition;
  }

  glGenBuffers(1, &ballzbuffer); 
  glBindBuffer(GL_ARRAY_BUFFER, ballzbuffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(ballz), ballz, GL_STATIC_DRAW);


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

  //for the rope
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, ropebuffer);
  glVertexAttribPointer(
                        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                        );
  

  //glVertexPointer(2, GL_FLOAT, 0, ropez);
  glDrawArrays(GL_LINES, 0, 2);

  //glDisableVertexAttribArray(0);
  glfwSwapBuffers(window);
  glfwPollEvents();
} // Check if the ESC key was pressed or the window was closed

while( !glfwWindowShouldClose(window) );
  // Close OpenGL window and terminate GLFW
  glfwDestroyWindow(window);
  glfwTerminate();
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &ballzbuffer); 
  glDeleteBuffers(1, &ropebuffer); 
  glDeleteVertexArrays(1, &VertexArrayID);
  exit(EXIT_SUCCESS);
}
