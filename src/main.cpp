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
   Shader lightSourceProgram("../assets/shaders/lightSource.vert", "../assets/shaders/lightSource.frag"); // Shader program for light sources
   
   // Vertex Array Buffer
   VAO VAO1;
   VAO1.Bind(); // Tells OpenGL to activate this VAO
   // Vertex Buffer Object
   // Contains vertex attributes such as position and color
   VBO VBO1(vertices, sizeof(vertices));
   EBO EBO1(indices, sizeof(indices)); // Responsible for storing indices on the GPU and drawing them in a specific order
   // Creating a second geometrical object 
   VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // Basically create a pointer to the VBO (position)

   // Creating our second VAO for another object
   VAO VAO2;
   VAO2.Bind();
   EBO EBO2(indices, sizeof(indices));
   VAO2.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // We reuse the first VBO since it already has the attributes we want

   VAO1.Unbind();
   VAO2.Unbind();
   VBO1.Unbind();
   EBO1.Unbind();
   EBO2.Unbind();

   // Get uniforms from lightSource fragment shader
   GLuint u_objColor = glGetUniformLocation(shaderProgram.ID, "u_objectColor");
   GLuint u_lightColor = glGetUniformLocation(shaderProgram.ID, "u_lightColor");

   glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer

   Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

   while(!glfwWindowShouldClose(window)) {
        // Specify color of background
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        // Clean the back buffer and assign the new color to it and update the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tell OpenGL which shader program we want to use
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        // Uniform Assignments
        glm::vec3 colorValue = {1.0f, 0.5f, 0.31f};
        glUniform3fv(u_objColor, 1, &colorValue[0]);
        glm::vec3 lightValue = {1.0f, 1.0f, 1.0f};
        glUniform3fv(u_lightColor, 1, &lightValue[0]); 

        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        
        // Activate our second shader program
        // Objects drawn under this activation will be using this shader
        lightSourceProgram.Activate();

        camera.Matrix(45.0f, 0.1f, 100.0f, lightSourceProgram, "camMatrix");

        GLuint modelLoc = glGetUniformLocation(lightSourceProgram.ID, "model");

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        // Create a model matrix to translate and scale our second object 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Do the same thing for the second VAO
        VAO2.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap back buffer with front buffer
        glfwSwapBuffers(window);
        // Makes sure our window is responsive (such as resizing it and moving it)
        glfwPollEvents();
   }

   // Deleting objects (memory management!)
   VAO1.Delete();
   VAO2.Delete();
   VBO1.Delete();
   EBO1.Delete();
   EBO2.Delete();
   shaderProgram.Delete();
   lightSourceProgram.Delete();

   glfwTerminate();
   return 0;
}
