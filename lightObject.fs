#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec4 gFragColor;
in vec2 TexCoords;
out vec4 FragColor;

uniform highp int toggleGouraudPhong = 0;
uniform bool invertSpec = false;
uniform bool addEmission = false;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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


void main()
{

    if (!bool(toggleGouraudPhong))
    {
        // calculate ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

        // calculate diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diffMult = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diffMult * vec3(texture(material.diffuse, TexCoords));

        // calculate specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 specTexVec;
        vec3 specular;
        if (invertSpec)
        {
            specTexVec = vec3(1.0f, 1.0f, 1.0f) - vec3(texture(material.specular, TexCoords));
            specular = light.specular * spec * specTexVec;
		}
        else 
        {
            specTexVec = vec3(texture(material.specular, TexCoords));
            specular = light.specular * spec * specTexVec;
		}        

        vec3 result = ambient + diffuse + specular;

        if (addEmission)
        {
            // calculate emission
            vec3 emission = vec3(texture(material.emission, TexCoords));
            result = result + emission;  
		}

        FragColor = vec4(result, 1.0);    
	}
    else
    {
        FragColor = gFragColor;
	}

}
