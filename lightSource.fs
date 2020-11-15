#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Light light;

uniform highp vec3 viewPos;
uniform bool outline = false;
uniform vec3 outlineColour;

vec4 CalcLight();


void main()
{
    if (outline)
    {
        FragColor = vec4(outlineColour, 1);
    }
    else
    {
        FragColor = CalcLight();
    }
}

vec4 CalcLight()
{
    // calculate ambient
    vec3 diffuse = light.diffuse;

    // calculate distance from fragment to light source
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (
        1.0 
        + (0.7 * distance) 
        + (1.8 * pow(distance, 2))
    );
    
    return vec4(diffuse * attenuation, 1.0);
}