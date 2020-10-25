#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform bool shadeInViewSpace = false;
uniform highp int toggleGouraudPhong = 0;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

void main()
{
    
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
        vec3 ambient = light.ambient * material.ambient;

        // calculate diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diffMult = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diffMult * material.diffuse);

        // calculate specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        // TODO: ScalarAttribute support for scale factor (linear/exponential)
        float shininess = pow(2, material.shininess);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = light.specular * (spec * material.specular);

        vec3 result = ambient + diffuse + specular;
        gFragColor = vec4(result, 1.0);
	}


    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
