#include <iostream>
#include "../headers/imgui/imgui.h"
#include "../headers/imgui/imgui_impl_glfw.h"
#include "../headers/imgui/imgui_impl_opengl3.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

#include "../headers/shaderClass.h"
#include "../headers/EBO.h"
#include "../headers/VBO.h"
#include "../headers/VAO.h"
#include "../headers/texture.h"
#include "../headers/camera.h"
#include "../headers/uniform.h"

#include "../include/stb/stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

const unsigned int width = 800;
const unsigned int height = 800;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
     glViewport(0, 0, width, height);
}

int main(int, char **)
{
     // Storing our values of each vertex in our coordinate space
     GLfloat vertices[] =
         {
             // positions          // normals           // texture coords
             -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
             0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
             0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
             -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
             -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

             -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

             -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

             0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

             -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
             0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
             0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
             -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
             -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

     glm::vec3 cubePositions[] = {
         glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(2.0f, 5.0f, -15.0f),
         glm::vec3(-1.5f, -2.2f, -2.5f),
         glm::vec3(-3.8f, -2.0f, -12.3f),
         glm::vec3(2.4f, -0.4f, -3.5f),
         glm::vec3(-1.7f, 3.0f, -7.5f),
         glm::vec3(1.3f, -2.0f, -2.5f),
         glm::vec3(1.5f, 2.0f, -2.5f),
         glm::vec3(1.5f, 0.2f, -1.5f),
         glm::vec3(-1.3f, 1.0f, -1.5f)};

     // Index Buffer
     /* GLuint indices[] =
      {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
      };*/

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
     Shader shaderProgram("../assets/shaders/default.vert", "../assets/shaders/default.frag");
     Shader lightSourceProgram("../assets/shaders/lightSource.vert", "../assets/shaders/lightSource.frag"); // Shader program for light sources
     
     Uniform shaderUniformManager(shaderProgram);

     Texture diffuseMap("../container2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     diffuseMap.texUnit(shaderProgram, "u_mat.diffuseMap", 0);

     Texture specularMap("../container2_specular.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
     specularMap.texUnit(shaderProgram, "u_mat.specularMap", 1);

     // Vertex Array Buffer
     VAO VAO1;
     VAO1.Bind(); // Tells OpenGL to activate this VAO
     // Vertex Buffer Object
     // Contains vertex attributes such as position and color
     VBO VBO1(vertices, sizeof(vertices));
     // EBO EBO1(indices, sizeof(indices)); // Responsible for storing indices on the GPU and drawing them in a specific order
     // Creating a second geometrical object
     VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0); // Basically create a pointer to the VBO (position)
     VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
     VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

     // Creating our second VAO for another object
     VAO VAO2;
     VAO2.Bind();
     // EBO EBO2(indices, sizeof(indices));
     VAO2.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0); // We reuse the first VBO since it already has the attributes we want

     VAO1.Unbind();
     VAO2.Unbind();
     VBO1.Unbind();
     // EBO1.Unbind();
     // EBO2.Unbind();

     // Get uniforms from default fragment shader
     GLuint u_objColor = glGetUniformLocation(shaderProgram.ID, "u_objectColor");
     GLuint u_viewPos = glGetUniformLocation(shaderProgram.ID, "u_viewPos");
     GLuint shininess = glGetUniformLocation(shaderProgram.ID, "u_mat.shininess");
     

     // Uniforms from light fragment shader
     GLuint u_lightColor = glGetUniformLocation(lightSourceProgram.ID, "u_lightColor");

     glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer

     Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

     glm::vec3 lightPos(1.2f, 2.0f, 1.0f);

     float deltaTime = 0.0f;
     float lastFrame = 0.0f;

     IMGUI_CHECKVERSION();
     ImGui::CreateContext();
     ImGuiIO &io = ImGui::GetIO();
     (void)io;
     ImGui::StyleColorsDark();
     ImGui_ImplGlfw_InitForOpenGL(window, true);
     ImGui_ImplOpenGL3_Init("#version 330");

     // ImGui Variables
     bool drawTriangle = true;
     glm::vec3 colorValue = {1.0f, 1.0f, 1.0f};

     glm::vec3 dirLightAmbientIntensity = {0.05f, 0.05f, 0.05f};
     glm::vec3 dirLightDiffuseIntensity = {0.4f, 0.4f, 0.4f};
     glm::vec3 dirLightSpecularIntensity = {0.1f, 0.1f, 0.1f};
     glm::vec3 dirLightVecDirection = {-0.2f, -1.0f, -0.3f};
     float shinyValue = 32.0f;

     glm::vec3 pointLightsPos[] = {
          glm::vec3(0.7f, 0.2f, 2.0f),
          glm::vec3(2.3f, -3.3f, -4.0f),
          glm::vec3(-4.0f, 2.0f, -12.0f),
          glm::vec3(0.0f, 0.0f, -3.0f)
     };

     float radius = 2.0f;

     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // Main Render Loop
     while (!glfwWindowShouldClose(window))
     {
          // Specify color of background
          glClearColor(0.0f, 0.0f, 0.30f, 1.0f);
          // Clean the back buffer and assign the new color to it and update the depth buffer
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();

          float crntFrame = glfwGetTime();
          deltaTime = crntFrame - lastFrame;
          lastFrame = crntFrame;

          // Tell OpenGL which shader program we want to use
          shaderProgram.Activate();

          if (!io.WantCaptureMouse)
               camera.Inputs(window);
          camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

          // Bind Texture(s) to their texture units
          glActiveTexture(GL_TEXTURE0);
          diffuseMap.Bind();
          glActiveTexture(GL_TEXTURE1);
          specularMap.Bind();
          // restore active texture to 0
          glActiveTexture(GL_TEXTURE0);
          // Uniform Assignments for shaderProgram
          glUniform3fv(u_objColor, 1, &colorValue[0]);
          glUniform3fv(u_viewPos, 1, &camera.Position[0]);

          shaderProgram.SetToVec3("dirLight.direction", &glm::vec3(-0.2f, -1.0f, -0.3f)[0]);
          shaderProgram.SetToVec3("dirLight.ambient", &dirLightAmbientIntensity[0]);
          shaderProgram.SetToVec3("dirLight.diffuse", &dirLightDiffuseIntensity[0]);
          shaderProgram.SetToVec3("dirLight.specular", &dirLightSpecularIntensity[0]);
          // Point Light 1
          shaderProgram.SetToVec3("pointLights[0].position", &pointLightsPos[0][0]);
          shaderProgram.SetToVec3("pointLights[0].ambient", &glm::vec3(0.05f, 0.05f, 0.05f)[0]);
          shaderProgram.SetToVec3("pointLights[0].diffuse", &glm::vec3(0.8f, 0.8f, 0.8f)[0]);
          shaderProgram.SetToVec3("pointLights[0].specular", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
          shaderProgram.SetToFloat("pointLights[0].constant", 1.0f);
          shaderProgram.SetToFloat("pointLights[0].linear", 0.09f);
          shaderProgram.SetToFloat("pointLights[0].quadratic", 0.032f);
          // Point Light 2
          shaderProgram.SetToVec3("pointLights[1].position", &pointLightsPos[1][0]);
          shaderProgram.SetToVec3("pointLights[1].ambient", &glm::vec3(0.05f, 0.05f, 0.05f)[0]);
          shaderProgram.SetToVec3("pointLights[1].diffuse", &glm::vec3(0.8f, 0.8f, 0.8f)[0]);
          shaderProgram.SetToVec3("pointLights[1].specular", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
          shaderProgram.SetToFloat("pointLights[1].constant", 1.0f);
          shaderProgram.SetToFloat("pointLights[1].linear", 0.09f);
          shaderProgram.SetToFloat("pointLights[1].quadratic", 0.032f);
          // Point Light 3
          shaderProgram.SetToVec3("pointLights[2].position", &pointLightsPos[2][0]);
          shaderProgram.SetToVec3("pointLights[2].ambient", &glm::vec3(0.05f, 0.05f, 0.05f)[0]);
          shaderProgram.SetToVec3("pointLights[2].diffuse", &glm::vec3(0.8f, 0.8f, 0.8f)[0]);
          shaderProgram.SetToVec3("pointLights[2].specular", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
          shaderProgram.SetToFloat("pointLights[2].constant", 1.0f);
          shaderProgram.SetToFloat("pointLights[2].linear", 0.09f);
          shaderProgram.SetToFloat("pointLights[2].quadratic", 0.032f);
          // Point Light 4
          shaderProgram.SetToVec3("pointLights[3].position", &pointLightsPos[3][0]);
          shaderProgram.SetToVec3("pointLights[3].ambient", &glm::vec3(0.05f, 0.05f, 0.05f)[0]);
          shaderProgram.SetToVec3("pointLights[3].diffuse", &glm::vec3(0.8f, 0.8f, 0.8f)[0]);
          shaderProgram.SetToVec3("pointLights[3].specular", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
          shaderProgram.SetToFloat("pointLights[3].constant", 1.0f);
          shaderProgram.SetToFloat("pointLights[3].linear", 0.09f);
          shaderProgram.SetToFloat("pointLights[3].quadratic", 0.032f);
          // Spot Light
          shaderProgram.SetToVec3("spotLight.position", &camera.Position[0]);
          shaderProgram.SetToVec3("spotLight.direction", &(camera.Position * camera.Orientation)[0]);
          shaderProgram.SetToVec3("spotLight.ambient", &glm::vec3(0.0f, 0.0f, 0.0f)[0]);
          shaderProgram.SetToVec3("spotLight.diffuse", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
          shaderProgram.SetToVec3("spotLight.specular", &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
          shaderProgram.SetToFloat("spotLight.constant", 1.0f);
          shaderProgram.SetToFloat("spotLight.linear", 0.09f);
          shaderProgram.SetToFloat("spotLight.quadratic", 0.032f);
          shaderProgram.SetToFloat("spotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
          shaderProgram.SetToFloat("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));
          


          // Updating the light's position over time (MUST be done before drawing, while shaderProgram is active)
          //lightPos.x = cos(glfwGetTime()) * radius;
          //lightPos.y = sin(glfwGetTime()) * radius;

          // Model matrix
          GLuint defaultModelLoc = glGetUniformLocation(shaderProgram.ID, "model");
          for (unsigned int i = 0; i < 10; i++)
          {
               glm::mat4 defaultModel = glm::mat4(1.0f);
               defaultModel = glm::translate(defaultModel, cubePositions[i]);
               float angle = 20.0f * i;
               defaultModel = glm::rotate(defaultModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
               glUniformMatrix4fv(defaultModelLoc, 1, GL_FALSE, glm::value_ptr(defaultModel));

               // Bind the VAO so OpenGL knows to use it
               VAO1.Bind();
               // Draw primitives, number of indices, datatype of indices, index of indices
               if (drawTriangle)
                    glDrawArrays(GL_TRIANGLES, 0, 36);
          }


          // Activate our second shader program
          // Objects drawn under this activation will be using this shader
          lightSourceProgram.Activate();

          camera.Matrix(45.0f, 0.1f, 100.0f, lightSourceProgram, "camMatrix");

          GLuint lightModelLoc = glGetUniformLocation(lightSourceProgram.ID, "model");

          for(unsigned int i = 0; i < 4; i++) 
          {
               // Create a model matrix to translate and scale our second object
               glm::mat4 lightModel = glm::mat4(1.0f);
               lightModel = glm::translate(lightModel, pointLightsPos[i]);
               lightModel = glm::scale(lightModel, glm::vec3(0.2));
               // Uniform assignments for lightSourceProgram
               glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
               // Do the same thing for the second VAO
               VAO2.Bind();
               if (drawTriangle)
                    glDrawArrays(GL_TRIANGLES, 0, 36);
          }
          // GUI STUFF
          ImGui::Begin("OpenGL Settings Panel");
          ImGui::Text("Tweaks");
          ImGui::Checkbox("Draw Triangle", &drawTriangle);
          ImGui::SliderFloat("Light Movement Radius", &radius, 1.0f, 4.0f, "%.1f");

          ImGui::Separator();

          ImGui::Text("Edit Colors");
          ImGui::ColorEdit3("Object Color", &colorValue[0], 0);
          ImGui::Separator();

          ImGui::Text("Edit Light Intensity");
          ImGui::SliderFloat("Shininess", &shinyValue, 0.0f, 64.0f, 0);
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

     // Deleting objects (memory management!)
     VAO1.Delete();
     VAO2.Delete();
     VBO1.Delete();
     // EBO1.Delete();
     // EBO2.Delete();
     shaderProgram.Delete();
     lightSourceProgram.Delete();
     // diffuseMap.Delete();
     // specularMap.Delete();

     glfwTerminate();
     return 0;
}
