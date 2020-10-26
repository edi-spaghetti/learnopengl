#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform bool shadeInViewSpace = false;
uniform highp int toggleGouraudPhong = 0;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    if (shadeInViewSpace)
    {
        FragPos = vec3(view * model * vec4(aPos, 1.0));
    }
    else
    {
        FragPos = vec3(model * vec4(aPos, 1.0));
    }

    if (bool(toggleGouraudPhong))
    {
        // calculate ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

        // calculate diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diffMult = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse *  vec3(texture(material.diffuse, TexCoords));

        // calculate specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

        vec3 result = ambient + diffuse + specular;
        gFragColor = vec4(result, 1.0);
	}

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
