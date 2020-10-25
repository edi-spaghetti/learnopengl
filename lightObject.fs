#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec4 gFragColor;
out vec4 FragColor;

uniform highp int toggleGouraudPhong = 0;
  
uniform highp vec3 objectColor;
uniform highp vec3 lightColor;
uniform highp vec3 lightPos;
uniform highp vec3 viewPos;

uniform highp float ambientStrength = 0.1;
uniform highp float specularStrength = 0.5;
uniform highp float shininessPower = 5.0;


void main()
{

    if (!bool(toggleGouraudPhong))
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
    else
    {
        FragColor = gFragColor;
	}

}
