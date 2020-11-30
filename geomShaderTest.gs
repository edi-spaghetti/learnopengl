#version 330 core
#define N 256
layout (points) in;
layout (points, max_vertices = N) out;

in VS_OUT {
	vec3 colour;
} gs_in[];
out vec3 fColour;

void buildHouse(vec4 position);
void growFlower(float c, float angle);
vec2 phyllotaxis(int n, float c, float angle, vec2 position);


void main() 
{		
	growFlower(0.01, 137.5);
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
	fColour = vec3(1, 1, 1);
	EmitVertex();
	EndPrimitive();
}

void growFlower(float c, float angle)
{
	fColour = gs_in[0].colour;
	vec4 position = gl_in[0].gl_Position;
	for (int n = 0; n < N; n++)
	{
		vec2 p = phyllotaxis(n, c, angle, position.xy);
		gl_Position = position + vec4(p.x, p.y, 0, 0) / 2;
		EmitVertex();
	}
	EndPrimitive();
}

vec2 phyllotaxis(int n, float c, float angle, vec2 position)
{
    float phy = n * angle;
	float r = c * sqrt(n);

	float x = r * cos(phy);
	float y = r * sin(phy);

	return vec2(x, y);
}