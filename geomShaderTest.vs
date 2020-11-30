#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aCol;

out VS_OUT {
	vec3 colour;
} vs_out;

void main()
{
	vs_out.colour = aCol;
	gl_Position = vec4(aPos.x, aPos.y, 0, 1);	
}