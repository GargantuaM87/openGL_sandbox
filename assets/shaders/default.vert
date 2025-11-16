#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTex;

out vec2 texCoord; // Output the coordinates of our texture
out vec3 color; // This will output this variable to the fragment shader

uniform float scale; // Is inputted from the CPU to all parallel threads on the GPU

void main()
{
    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
    color = aColor;
    texCoord = aTex;
}