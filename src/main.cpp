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

unsigned int LoadCubeMap(std::vector<std::string> faces)
{
     unsigned int textureID;
     glGenTextures(1, &textureID);
     glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

     int width, height, nrChannels;
     for(unsigned int i = 0; i < faces.size(); i++)
     {
          unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
          if(data)
          {
               glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
               stbi_image_free(data);
          }
          else
          {
               std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
               stbi_image_free(data);
          }
     }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

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
          // positions     // texCoords
          -1.0f,  1.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f,

          -1.0f,  1.0f,  0.0f, 1.0f,
          1.0f, -1.0f,  1.0f, 0.0f,
          1.0f,  1.0f,  1.0f, 1.0f
     };
     float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
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
     // shaders
     Shader lightSourceProgram("../assets/shaders/lightSource.vert", "../assets/shaders/lightSource.frag"); // Shader program for light sources
     Shader simpleShader("../assets/shaders/simple.vert", "../assets/shaders/simple.frag");
     Shader skyboxShader("../assets/shaders/skybox.vert", "../assets/shaders/skybox.frag");
     
     // textures
     TextureUnit cubeTexture("../container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     TextureUnit floorTexture("../marble.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     // cubemap texture
     unsigned int texID;
     glGenTextures(1, &texID);
     glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

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
     quadVAO.LinkAttrib(quadVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
     quadVAO.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
     quadVAO.Unbind();
     // skybox geometry
     VAO skyboxVAO;
     VBO skyboxVBO(skyboxVertices, sizeof(skyboxVertices));
     skyboxVAO.Bind();
     skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
     skyboxVAO.Unbind();


     // frame buffer
     unsigned int fbo;
     glGenFramebuffers(1, &fbo);
     glBindFramebuffer(GL_FRAMEBUFFER, fbo);

     // texture for frame buffer
     unsigned int texture;
     glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_2D, texture);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
     glBindTexture(GL_TEXTURE_2D, 0); 

     // render buffer
     unsigned int rbo;
     glGenRenderbuffers(1, &rbo);
     glBindRenderbuffer(GL_RENDERBUFFER, rbo);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
     glBindRenderbuffer(GL_RENDERBUFFER, 0);

     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
          std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
     glBindFramebuffer(GL_FRAMEBUFFER, 0);


     glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer
     glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND); // enable alpha blending
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

     glm::vec3 lightPos(0.5f, 0.5f, 0.0f);
     float radius = 3.0f;

     float deltaTime = 0.0f;
     float lastFrame = 0.0f;
     
     // cubemap textures
     std::vector<std::string> cubemaps = {
          "../assets/skybox/left.jpg",
          "../assets/skybox/right.jpg",
          "../assets/skybox/bottom.jpg",
          "../assets/skybox/top.jpg",
          "../assets/skybox/back.jpg",
           "../assets/skybox/front.jpg"
     };
     unsigned int cubemapTexture = LoadCubeMap(cubemaps);

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
          glBindFramebuffer(GL_FRAMEBUFFER, fbo);
          glEnable(GL_DEPTH_TEST);

          glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();

          float crntFrame = glfwGetTime();
          deltaTime = crntFrame - lastFrame;
          lastFrame = crntFrame;

          glDepthMask(GL_FALSE);
          skyboxShader.Activate();
          camera.Matrix(45.0f, 0.1f, 100.0f, skyboxShader, "camMatrix");
          glm::mat4 skyboxModel = glm::mat4(1.0f);
          skyboxShader.SetToMat4("modelMatrix", skyboxModel);
          skyboxVAO.Bind();
          glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
          glDrawArrays(GL_TRIANGLES, 0, 36);
          glDepthMask(GL_TRUE);

          lightSourceProgram.Activate();
          camera.Matrix(45.0f, 0.1f, 100.0f, lightSourceProgram, "camMatrix");

          if (!io.WantCaptureMouse)
               camera.Inputs(window);

           // floor
         /* planeVAO.Bind();
          floorTexture.Bind();
          glm::mat4 floorModel = glm::mat4(1.0f);
          lightSourceProgram.SetToMat4("model", floorModel);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          floorTexture.Unbind();*/
          
          glm::mat4 model = glm::mat4(1.0f);

          // cube
          cubeVAO.Bind();
          glActiveTexture(GL_TEXTURE0);
          cubeTexture.Bind();

          model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
          lightSourceProgram.SetToMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          model = glm::mat4(1.0f);
          model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
          lightSourceProgram.SetToMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);


          glBindFramebuffer(GL_FRAMEBUFFER, 0);
          glDisable(GL_DEPTH_TEST);
          glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);

          simpleShader.Activate();
          quadVAO.Bind();
          glBindTexture(GL_TEXTURE_2D, texture);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          
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
     glDeleteRenderbuffers(1, &rbo);
     glDeleteFramebuffers(1, &fbo);
     
     glfwTerminate();
     return 0;
}
