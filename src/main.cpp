#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

#include "../headers/shaderClass.h"
#include "../headers/EBO.h"
#include "../headers/VBO.h"
#include "../headers/VAO.h"
#include "../headers/texture.h"
#include "../headers/camera.h"

#include "../include/stb/stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

const unsigned int width = 800;
const unsigned int height = 800;

int main(int, char**){
   // Storing our values of each vertex in our coordinate space
   GLfloat vertices[] = 
   {    // POSITIONS            // COLORS      // TEXT COORD
     -0.5f,  0.0f, 0.5f,    0.8f, 0.3f, 0.02f,  0.0f, 0.0f, 
     -0.5,   0.0f,-0.5f,    0.8f, 0.3f, 0.02f,  5.0f, 1.0f, 
      0.5f,  0.0f,-0.5f,    1.0f, 6.0f, 0.32f,  0.0f, 0.0f, 
      0.5f, -0.0,  0.6f,    0.9f, 0.45, 0.17f,  5.0f, 0.0f,
      0.0f,  0.8f, 0.0f,    0.92f,0.86f,0.76f,   2.5f, 5.0f
   };

   // Index Buffer
   GLuint indices[] = 
   { 
     0, 1, 2,
     0, 2, 3,
     0, 1, 4,
     1, 2, 4,
     2, 3, 4,
     3, 0, 4
   };
   
   GLFWwindow* window;

   if(!glfwInit()) {
        return -1;
   }
   // Create the window that appears on the screen
   window = glfwCreateWindow(width, height, "Window", NULL, NULL);

   // Tells GLFW to add the window to the current context
   // A context being an object that encapsulates the OpenGL state machine
   glfwMakeContextCurrent(window);

   if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Couldn't load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
   }
   // Parses the fragment and vertex shader files and wraps them into a shader program
   // The files are compiled to an intermediary language then translated into specific instructions for the GPU
   Shader shaderProgram("../assets/shaders/default.vert", "../assets/shaders/default.frag");
   
   // Vertex Array Buffer
   // Stores pointers to VBOs
   // Defines how we can access our VBOs and their attributes
   VAO VAO1;
   VAO1.Bind(); // Tells OpenGL to activate this VAO

   // Vertex Buffer Object
   // Contains vertex attributes such as position and color
   VBO VBO1(vertices, sizeof(vertices));
   EBO EBO1(indices, sizeof(indices));

   VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // Basically create a pointer to the VBO (position)
   VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Pointer to colors
   VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Pointer to texture coordinates
   VAO1.Unbind();
   VBO1.Unbind();
   EBO1.Unbind();

   Texture myTex("../brick_12-512x512.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
   myTex.texUnit(shaderProgram, "tex0", 0);

   glEnable(GL_DEPTH_TEST);

   Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

   while(!glfwWindowShouldClose(window)) {
        // Specify color of background
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which shader program we want to use
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        myTex.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap back buffer with front buffer
        glfwSwapBuffers(window);
        // Makes sure our window is responsive (such as resizing it and moving it)
        glfwPollEvents();
   }

   // Deleting objects (memory management!)
   VAO1.Delete();
   VBO1.Delete();
   EBO1.Delete();
   shaderProgram.Delete();
   myTex.Delete();

   glfwTerminate();
   return 0;
}
