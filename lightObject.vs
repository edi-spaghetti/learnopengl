#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform bool shadeInViewSpace = false;
uniform highp int toggleGouraudPhong = 0;

uniform highp vec3 objectColor;
uniform highp vec3 lightColor;
uniform highp vec3 lightPos;
uniform highp vec3 viewPos;

uniform highp float ambientStrength = 0.1;
uniform highp float specularStrength = 0.5;
uniform highp float shininessPower = 5.0;

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
        vec3 ambient = ambientStrength * lightColor;

        // calculate diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diffuseMultiplier = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffuseMultiplier * lightColor;

        // calculate specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float shininess = pow(2, shininessPower);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * objectColor;
        gFragColor = vec4(result, 1.0);    
	}


    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
