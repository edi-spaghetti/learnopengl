#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 posColour;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    posColour = vec4(aPos, 1.0);
}