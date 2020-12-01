#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in IO_BLOCK {
	vec3 Normal;
} gs_in[];

uniform mat4 projection;
const float MAGNITUDE = 0.4f;

void generateLine(int index);

void main()
{
	for (int i = 0; i < 3; i++)
	{
		generateLine(i);
	}
}

void generateLine(int index)
{
	for (int i = 0; i < 2; i++)
	{
		vec4 end = vec4(gs_in[index].Normal, 0) * MAGNITUDE * i;
		gl_Position = projection * (gl_in[index].gl_Position + end);
		EmitVertex();
	}
	EndPrimitive();
}