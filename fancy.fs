#version 330 core
out vec4 FragColor;

in vec4 posColour;

void main()
{
    FragColor = posColour;
}