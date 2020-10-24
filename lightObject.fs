#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    // calculate ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // calculate diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuseMultiplier = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseMultiplier * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
