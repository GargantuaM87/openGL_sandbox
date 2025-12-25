#include <iostream>
#include "../headers/imgui/imgui.h"
#include "../headers/imgui/imgui_impl_glfw.h"
#include "../headers/imgui/imgui_impl_opengl3.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <filesystem>

#include "../headers/shaderClass.h"
#include "../headers/EBO.h"
#include "../headers/VBO.h"
#include "../headers/VAO.h"
#include "../headers/texture.h"
#include "../headers/camera.h"
#include "../headers/uniform.h"
#include "../headers/model.h"

#include "../include/stb/stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include <map>

const unsigned int width = 800;
const unsigned int height = 800;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
     glViewport(0, 0, width, height);
}

int main(int, char **)
{
     // set up vertex data (and buffer(s)) and configure vertex attributes
     // ------------------------------------------------------------------
     float cubeVertices[] = {
         // positions          // texture Coords
         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
         -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

         -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
     float planeVertices[] = {
         // positions          
         // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
         -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
         -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

         5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
         -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
         5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
     float quadVertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f
     };

     GLFWwindow *window;

     if (!glfwInit())
     {
          return -1;
     }
     // Create the window that appears on the screen
     window = glfwCreateWindow(width, height, "Window", NULL, NULL);

     // Tells GLFW to add the window to the current context
     // A context being an object that encapsulates the OpenGL state machine
     glfwMakeContextCurrent(window);

     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
          std::cout << "Couldn't load OpenGL" << std::endl;
          glfwTerminate();
          return -1;
     }
     // Parses the fragment and vertex shader files and wraps them into a shader program
     // The files are compiled to an intermediary language then translated into specific instructions for the GPU
     Shader lightSourceProgram("../assets/shaders/lightSource.vert", "../assets/shaders/lightSource.frag"); // Shader program for light sources
     Shader depthTestProgram("../assets/shaders/depthTest.vert", "../assets/shaders/depthTest.frag");
     Shader stencilTestProgram("../assets/shaders/depthTest.vert", "../assets/shaders/shaderSingleColor.frag");

     TextureUnit cubeTexture("../metal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     cubeTexture.texUnit(depthTestProgram, "texture1", 0);
     TextureUnit floorTexture("../marble.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     floorTexture.texUnit(depthTestProgram, "texture1", 0);
     TextureUnit grassTexture("../blending_transparent_window.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

     // cube VAO
     unsigned int cubeVAO, cubeVBO;
     glGenVertexArrays(1, &cubeVAO);
     glGenBuffers(1, &cubeVBO);
     glBindVertexArray(cubeVAO);
     glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
     glBindVertexArray(0);
     // plane VAO
     unsigned int planeVAO, planeVBO;
     glGenVertexArrays(1, &planeVAO);
     glGenBuffers(1, &planeVBO);
     glBindVertexArray(planeVAO);
     glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
     glBindVertexArray(0);
     // leaf quad VAO
     unsigned int grassQuadVAO, grassQuadVBO;
     glGenVertexArrays(1, &grassQuadVAO);
     glGenBuffers(1, &grassQuadVBO);
     glBindVertexArray(grassQuadVAO);
     glBindBuffer(GL_ARRAY_BUFFER, grassQuadVBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
     glBindVertexArray(0);

     glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer
     glEnable(GL_BLEND); // enable alpha blending
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

     glm::vec3 lightPos(0.5f, 0.5f, 0.0f);
     float radius = 3.0f;

     float deltaTime = 0.0f;
     float lastFrame = 0.0f;

     // grass positions
     std::vector<glm::vec3> windows;
     windows.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
     windows.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
     windows.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
     windows.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
     windows.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));  

     IMGUI_CHECKVERSION();
     ImGui::CreateContext();
     ImGuiIO &io = ImGui::GetIO();
     (void)io;
     ImGui::StyleColorsDark();
     ImGui_ImplGlfw_InitForOpenGL(window, true);
     ImGui_ImplOpenGL3_Init("#version 330");

     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // Main Render Loop
     while (!glfwWindowShouldClose(window))
     {
          // Specify color of background
          glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
          // Clean the back buffer and assign the new color to it and update the depth buffer
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();

          float crntFrame = glfwGetTime();
          deltaTime = crntFrame - lastFrame;
          lastFrame = crntFrame;

          depthTestProgram.Activate();
          camera.Matrix(45.0f, 0.1f, 100.0f, depthTestProgram, "camMatrix");

          if (!io.WantCaptureMouse)
               camera.Inputs(window);

          std::map<float, glm::vec3> sorted;
          for(unsigned int i = 0; i < windows.size(); i++)
          {
               float distance = glm::length2(camera.Position - windows[i]);
               sorted[distance] = windows[i];
          }

           // floor
          glBindVertexArray(planeVAO);
          floorTexture.Bind();
          glm::mat4 floorModel = glm::mat4(1.0f);
          depthTestProgram.SetToMat4("model", floorModel);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          floorTexture.Unbind();
          
          glm::mat4 model = glm::mat4(1.0f);

          // cube
          glBindVertexArray(cubeVAO);
          glActiveTexture(GL_TEXTURE0);
          cubeTexture.Bind();

          model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
          depthTestProgram.SetToMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          model = glm::mat4(1.0f);
          model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
          depthTestProgram.SetToMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          glBindVertexArray(grassQuadVAO);
          grassTexture.Bind();
          for(std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
               model = glm::mat4(1.0f);
               model = glm::translate(model, glm::vec3(it->second.x, it->second.y, it->second.z + 2.5f));
               depthTestProgram.SetToMat4("model", model);
               glDrawArrays(GL_TRIANGLES, 0, 6);
          }
          glBindVertexArray(0);
          
          // GUI STUFF
          ImGui::Begin("OpenGL Settings Panel");
          ImGui::Text("Tweaks");
     
          ImGui::End();

          ImGui::Render();
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

          // Swap back buffer with front buffer
          glfwSwapBuffers(window);
          // Makes sure our window is responsive (such as resizing it and moving it)
          glfwPollEvents();
     }

     ImGui_ImplOpenGL3_Shutdown();
     ImGui_ImplGlfw_Shutdown();
     ImGui::DestroyContext();

     glDeleteVertexArrays(1, &cubeVAO);
     glDeleteVertexArrays(1, &planeVAO);
     glDeleteBuffers(1, &cubeVBO);
     glDeleteBuffers(1, &planeVBO);

     glfwTerminate();
     return 0;
}
