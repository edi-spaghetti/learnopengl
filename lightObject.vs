#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform highp int toggleGouraudPhong = 0;
uniform highp int lightingType;
const int POINT = 0;
const int DIRECTIONAL = 1;
const int SPOTLIGHT = 2;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};
uniform Light light;

uniform highp vec3 viewPos;

out vec3 Normal;
out vec3 FragPos;
out vec4 gFragColor;
out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    Normal = normalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


void deprecatedGouraudShading()
{
    if (bool(toggleGouraudPhong))
    {
        // calculate light direction
        vec3 lightDir;
        float attenuation;
        if (lightingType == POINT) 
        {
            lightDir = normalize(light.position - FragPos);

            // calculate attenuation
            float distance = length(light.position - FragPos);
            attenuation = 1.0 / (
                light.constant + 
                light.linear * distance + 
                light.quadratic * pow(distance, 2)
            );
        }
        if (lightingType == DIRECTIONAL) lightDir = normalize(-light.direction);

        // calculate ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        if (lightingType == POINT) ambient *= attenuation;

        // calculate diffuse
        vec3 norm = normalize(Normal);
        float diffMult = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse *  vec3(texture(material.diffuse, TexCoords));
        if (lightingType == POINT) diffuse *= attenuation;

        // calculate specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        if (lightingType == POINT) specular *= attenuation;

        vec3 result = ambient + diffuse + specular;
        gFragColor = vec4(result, 1.0);
	}
}