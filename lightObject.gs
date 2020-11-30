#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in IO_BLOCK {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} gs_in[];

out IO_BLOCK {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} gs_out;

uniform float time;

vec4 explode(vec4 position, vec3 normal);

void main()
{
	for (int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;
		gs_out.Normal = gs_in[i].Normal;
		gs_out.FragPos = gs_in[i].FragPos;
		gs_out.TexCoords = gs_in[i].TexCoords;	
		EmitVertex();
	}
	EndPrimitive();
}


vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2;
	vec3 direction = normal * ((sin(time) + 1) / 2) * magnitude;
	return position + vec4(direction, 0);
}