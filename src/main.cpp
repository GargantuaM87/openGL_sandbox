#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main(int, char**){
   // Storing our values of each vertex in our coordinate space
   GLfloat vertices[] = 
   {
     -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Bottom left corner of triangle
     0.5, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Bottom right corner of triangle
     0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.00f, // Top middle of triangle
     -0.5f / 2, 0.5 * float(sqrt(3)) / 6, 0.00, // Inner left
     0.5f / 2, 0.5 * float(sqrt(3)) / 6, 0.00,  // Inner right
     0.0f, -0.5 * float(sqrt(3)) / 3, 0.00f // Inner down
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
   // This is an unsigned-int 
   // glShaderSource replaces the source code of vertexShader with the source code of vertexShaderSource (its memory address)
   // GPU can't understand source code, so we compile the shader as well
   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   // Wrap the fragment and vertex shaders into a shader program
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   // Delete the shaders since we now have them in our shader program (memory management!)
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   // Sending stuff between the GPU and CPU is slow, so we want to send data through big batches called buffers
   GLuint VAO, VBO, EBO;

   // Generate the VAO and VBO with only 1 object each
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   // Bind the VBO specifying its a GL_ARRAY_BUFFER
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   // Introduce the vertices into the VBO
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Necessary for OpenGL to read the VAO
   // Way of communicating with a vertex shader from the outside
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   // Bind both the VBO and VAO to 0
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // Specify the color of the background
   glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

   while(!glfwWindowShouldClose(window)) {
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Makes sure our window is responsive (such as resizing it and moving it)
        glfwPollEvents();
   }

   // Deleting objects (memory management!)
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
   glDeleteProgram(shaderProgram);

   glfwTerminate();
   return 0;
}
