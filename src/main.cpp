#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

#include "../headers/shaderClass.h"
#include "../headers/EBO.h"
#include "../headers/VBO.h"
#include "../headers/VAO.h"

int main(int, char**){
   // Storing our values of each vertex in our coordinate space
   GLfloat vertices[] = 
   {         // POSITIONS                           // COLORS
     -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    0.8f, 0.3f, 0.02f,   // Bottom left corner of triangle
     0.5, -0.5f * float(sqrt(3)) / 3, 0.0f,      0.8f, 0.3f, 0.02f,  // Bottom right corner of triangle
     0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.00f, 1.0f, 6.0f, 0.32f,  // Top middle of triangle
     -0.5f / 2, 0.5 * float(sqrt(3)) / 6, 0.00,  0.9f, 0.45, 0.17f,  // Inner left
     0.5f / 2, 0.5 * float(sqrt(3)) / 6, 0.00,   0.9f, 0.45f, 0.17f,  // Inner right
     0.0f, -0.5 * float(sqrt(3)) / 3, 0.00f,     0.8f, 0.3f,  0.02f   // Inner down
   };

   // Index Buffer
   GLuint indices[] = 
   { 
     0, 3, 5, // lower left triangle
     3, 2, 4, // lower right triangle
     5, 4, 1 // upper triangle
   };


   GLFWwindow* window;

   if(!glfwInit()) {
        return -1;
   }
   // Create the window that appears on the screen
   window = glfwCreateWindow(800, 800, "Window", NULL, NULL);

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

   VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0); // Basically create a pointer to the VBO
   VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
   VAO1.Unbind();
   VBO1.Unbind();
   EBO1.Unbind();

   GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


   while(!glfwWindowShouldClose(window)) {
        // Specify color of background
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which shader program we want to use
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
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

   glfwTerminate();
   return 0;
}
