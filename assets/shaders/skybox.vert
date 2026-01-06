#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
    TexCoords = vec3(aPos.x, -aPos.y, aPos.z);
    gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);
}