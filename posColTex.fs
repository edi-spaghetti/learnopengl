#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D container;
uniform sampler2D awesomeface;

void main()
{
    FragColor = mix(texture(container, TexCoord), texture(awesomeface, TexCoord), 0.2);
}