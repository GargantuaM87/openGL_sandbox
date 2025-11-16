#version 330 core
out vec4 FragColor;

// Taking input from the vertex shader
// It is necessary that the variable names are the same in both shaders
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, texCoord);
}