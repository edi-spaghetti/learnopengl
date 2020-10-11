#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

void main()
{
   gl_Position = vec4(aPos, 1.0);
   vertexColor = vec4(
           88.0f  / 255.0f,
           24.0f  / 255.0f,
           69.0f  / 255.0f,
           255.0f / 255.0f
       );
}