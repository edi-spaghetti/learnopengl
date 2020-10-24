#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.5;
uniform float shininessPower = 5.0;


void main()
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
    FragColor = vec4(result, 1.0);
}
