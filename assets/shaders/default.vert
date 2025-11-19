#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTex;

out vec2 texCoord; // Output the coordinates of our texture
out vec3 color; // This will output this variable to the fragment shader

 // Uniforms are inputted from the CPU to all parallel threads on the GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    color = aColor;
    texCoord = aTex;
}