#version 330 core
out vec4 FragColor;

// Taking input from the vertex shader
// It is necessary that the variable names are the same in both shaders

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

void main()
{
    FragColor = vec4(u_lightColor * u_objectColor, 1.0);
}