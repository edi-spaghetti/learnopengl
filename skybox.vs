#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	// invert position because cubemap coordinate system is left handed.
	TexCoords = -aPos;
	vec4 pos = projection * view * vec4(aPos, 1);
	gl_Position = pos.xyww;
}