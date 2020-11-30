#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 colour;
} gs_in[];
out vec3 fColour;

void buildHouse(vec4 position);

void main() 
{
	buildHouse(gl_in[0].gl_Position);
}

void buildHouse(vec4 position) {
	fColour = gs_in[0].colour;
	gl_Position = position + vec4(-0.2, -0.2, 0, 0);
	EmitVertex();
	gl_Position = position + vec4( 0.2, -0.2, 0, 0);
	EmitVertex();
	gl_Position = position + vec4(-0.2,  0.2, 0, 0);
	EmitVertex();
	gl_Position = position + vec4( 0.2,  0.2, 0, 0);
	EmitVertex();
	gl_Position = position + vec4( 0.0,  0.4, 0, 0);
	EmitVertex();
	EndPrimitive();
}