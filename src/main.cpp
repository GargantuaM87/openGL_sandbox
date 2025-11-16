#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>


#include "../headers/shaderClass.h"
#include "../headers/EBO.h"
#include "../headers/VBO.h"
#include "../headers/VAO.h"
#include "../include/stb/stb_image.h"


int main(int, char**){
   // Storing our values of each vertex in our coordinate space
   GLfloat vertices[] = 
   {    // POSITIONS            // COLORS      // TEXT COORD
     -0.5f, -0.5f, 0.0f,    0.8f, 0.3f, 0.02f,  0.0f, 0.0f, // Bottom left corner of square
     -0.5,   0.5f, 0.0f,    0.8f, 0.3f, 0.02f,  0.0f, 1.0f, // Top left corner of square
      0.5f,  0.5f, 0.0f,    1.0f, 6.0f, 0.32f,  1.0f, 1.0f, // Top right corner of square
      0.5f, -0.5,  0.00,    0.9f, 0.45, 0.17f,  1.0f, 0.0f // Bottom right corner of square
   };

   // Index Buffer
   GLuint indices[] = 
   { 
     0, 2, 1, // Upper triangle
     0, 3, 2 // Lower triangle
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

   VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // Basically create a pointer to the VBO (position)
   VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Pointer to colors
   VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Pointer to texture coordinates
   VAO1.Unbind();
   VBO1.Unbind();
   EBO1.Unbind();

   GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

   // Texture
   int widthImg, heightImg, numColCh;
   unsigned char* bytes = stbi_load("../brick_12-512x512.png", &widthImg, &heightImg, &numColCh, STBI_rgb_alpha);

   GLuint texture;
   glGenTextures(1, &texture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);

   // Changing the texture's parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
   // glTextParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
   glGenerateMipmap(GL_TEXTURE_2D);

   stbi_image_free(bytes);
   glBindTexture(GL_TEXTURE_2D, 0);

   GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
   shaderProgram.Activate();
   glUniform1i(tex0Uni, 0);

   while(!glfwWindowShouldClose(window)) {
        // Specify color of background
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which shader program we want to use
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

   glDeleteTextures(1, &texture);

   glfwTerminate();
   return 0;
}
