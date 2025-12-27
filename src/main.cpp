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
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
     };
     float planeVertices[] = {
         // positions          
         // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
         -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
         -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
         
         5.0f, -0.5f, -5.0f, 2.0f, 2.0f,
         -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
         5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
         
     };
     float quadVertices[] = {
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
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

     // cube geometry 
     VAO cubeVAO;
     VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
     cubeVAO.Bind();
     cubeVAO.LinkAttrib(cubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
     cubeVAO.LinkAttrib(cubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void *)(3 * sizeof(float)));
     cubeVAO.Unbind();
     // plane geometry
     VAO planeVAO;
     VBO planeVBO(planeVertices, sizeof(planeVertices));
     planeVAO.Bind();
     planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
     planeVAO.LinkAttrib(planeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
     planeVAO.Unbind();
     // quad geometry
     VAO quadVAO;
     VBO quadVBO(quadVertices, sizeof(quadVertices));
     quadVAO.Bind();
     quadVAO.LinkAttrib(quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
     quadVAO.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
     quadVAO.Unbind();

     glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer
     glEnable(GL_CULL_FACE);
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
          planeVAO.Bind();
          floorTexture.Bind();
          glm::mat4 floorModel = glm::mat4(1.0f);
          depthTestProgram.SetToMat4("model", floorModel);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          floorTexture.Unbind();
          
          glm::mat4 model = glm::mat4(1.0f);

          // cube
          cubeVAO.Bind();
          glActiveTexture(GL_TEXTURE0);
          cubeTexture.Bind();

          model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
          depthTestProgram.SetToMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          model = glm::mat4(1.0f);
          model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
          depthTestProgram.SetToMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          quadVAO.Bind();
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

     cubeVAO.Delete();
     cubeVBO.Delete();
     planeVAO.Delete();
     planeVBO.Delete();
     quadVAO.Delete();
     quadVBO.Delete();
     
     glfwTerminate();
     return 0;
}
