#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D container;
uniform sampler2D awesomeface;

uniform float zoom;
uniform float alpha;

float mapToRange(float X, float A, float B, float C, float D)
{
	return (X - A) * ((D - C) / (B - A)) + C;
}

void main()
{
    vec2 zoomedTexCoord = vec2(
        mapToRange(TexCoord.s, 0.0, 1.0, 0.0 + zoom, 1.0 - zoom), 
        mapToRange(TexCoord.t, 0.0, 1.0, 0.0 + zoom, 1.0 - zoom)
    );

    FragColor = mix(
        texture(container, zoomedTexCoord), 
        texture(awesomeface, zoomedTexCoord), 
        alpha
    );
}