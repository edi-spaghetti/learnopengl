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


void main()
{
    // calculate ambient
    vec3 ambient = light.ambient * 2;

    // calculate distance from fragment to light source
    float distance = sqrt(
        pow(light.position.x - FragPos.x, 2) +
        pow(light.position.y - FragPos.y, 2) +
        pow(light.position.z - FragPos.z, 2)
	);

    vec3 result = ambient * (1.0f / pow(distance, 2));
    FragColor = vec4(result, 1.0);
}