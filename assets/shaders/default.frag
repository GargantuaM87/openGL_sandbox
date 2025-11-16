#version 330 core
out vec4 FragColor;

// Taking input from the vertex shader
// It is necessary that the variable names are the same in both shaders
in vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}