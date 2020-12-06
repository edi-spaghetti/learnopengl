#version 330 core
#define MAX_LIGHTS 6
#define POINT 0
#define DIRECTIONAL 1
#define SPOTLIGHT 2

// IO
in IO_BLOCK {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} fs_in;
out vec4 FragColor;

// struct declarations
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    float opacity;
};

struct Light {
    int type;
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerBeam;
    float outerBeam;
};

// uniforms
uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform highp int toggleGouraudPhong = 0;
uniform bool invertSpec = false;
uniform bool addEmission = false;
uniform bool animateEmission = false;
uniform bool doBlinn = false;
uniform float time;
uniform int numLights;
uniform highp vec3 viewPos;
uniform samplerCube skybox;
uniform bool gammaCorrection = true;

// function declarations
vec4 CalcLight();
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcAmbient(Light light);
vec3 CalcDiffuse(Light light, vec3 lgtDirection, vec3 normal);
vec3 CalcSpecular(Light light, vec3 lgtDirection, vec3 viewDirection, vec3 normal);
vec3 CalcEmission(bool animated);
vec3 CalcReflection(vec3 normal, vec3 viewDir);
vec3 CalcRefraction(float iRefrIndex, float oRefrIndex, vec3 normal, vec3 viewDir);
float CalcAttenuation(Light light, float dist);

// constants
const float kPi = 3.14159265;


void main() {
     FragColor = CalcLight();
}


vec4 CalcLight() {

    // common properties
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    // init container for results
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    // cycle through lights and apply by type
    for (int i = 0; i < numLights; i++)
    {
        if (lights[i].type == DIRECTIONAL) result += CalcDirLight(lights[i], norm, viewDir);
        if (lights[i].type == POINT) result +=  CalcPointLight(lights[i], norm, fs_in.FragPos, viewDir);
        if (lights[i].type == SPOTLIGHT) result += CalcSpotLight(lights[i], norm, fs_in.FragPos, viewDir);
	}

    // add reflection
    result += CalcReflection(norm, viewDir);

    // add refraction
    // result = CalcRefraction(1, 1.52, norm, viewDir);

    // apply emission (if any)
    if (addEmission) result += CalcEmission(animateEmission);

    return vec4(result, material.opacity);
}


vec3 CalcAmbient(Light light) {
    return light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
}


vec3 CalcDiffuse(Light light, vec3 lgtDirection, vec3 normal) {
    float diffMult = max(dot(normal, lgtDirection), 0.0);
    return light.diffuse * diffMult * vec3(texture(material.diffuse, fs_in.TexCoords));
}


vec3 CalcSpecular(Light light, vec3 lgtDirection, vec3 viewDirection, vec3 normal) {

    vec3 pivotVector;
    float spec;
    if (doBlinn)
    {
        float energyConservation = (8.0 + material.shininess) / (8.0 * kPi);
        vec3 halfwayDir = normalize(lgtDirection + viewDirection);
        spec = energyConservation * pow(max(dot(normal, halfwayDir), 0), material.shininess);
    }
    else
    {
       float energyConservation = (2.0 + material.shininess) / (2.0 * kPi);
       vec3 reflectDir = reflect(-lgtDirection, normal);
       spec = energyConservation * pow(max(dot(viewDirection, reflectDir), 0), material.shininess);
    }

    vec3 specTexVec = vec3(texture(material.specular, fs_in.TexCoords));
    if (invertSpec) specTexVec = vec3(1.0f, 1.0f, 1.0f) - specTexVec;

    return light.specular * spec * specTexVec;
}


vec3 CalcReflection(vec3 normal, vec3 viewDir)
{
    vec3 R = reflect(viewDir, normal);
    vec3 reflection = vec3(texture(skybox, R)).rgb;

    return reflection * (material.shininess / 256);
}


vec3 CalcRefraction(float iRefrIndex, float oRefrIndex, vec3 normal, vec3 viewDir)
{
    float ratio = iRefrIndex / oRefrIndex;
    vec3 I = -normalize(fs_in.FragPos - viewPos);
    vec3 R = refract(I, -normal, ratio);
    return texture(skybox, R).rgb;
}



vec3 CalcEmission(bool animated) {

    vec3 emission = vec3(texture(material.emission, fs_in.TexCoords)).rgb;
    if (animated)
    {
        float loopTime = 10.0;  // seconds
        float currentTime = (mod(time, loopTime) / loopTime) / 2;
        float startTime;
        float endTime;
        float fallOff = 0.0f;
        float duration = 0.25f;

        // TOOD: have this set by a uniform
        vec3 emissionColour = vec3(
            31.0f / 255.0f, 
            109.0f / 255.0f, 
            31.0f / 255.0f
        );

        for (int i = 0; i < 3; i++)
        {
            startTime = emission[i] - 0.5f;
            if (0.001f < emission[i] && startTime < currentTime)
            {
                endTime = startTime + duration;
                float newFallOff = max(endTime - currentTime, 0.0f);
                fallOff = max(newFallOff, fallOff);                    
			}
		}
        emission = emissionColour * fallOff;
	}

    return emission;
}


vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {
    
    // properties
    vec3 lightDir = normalize(-light.direction);

    // calculate
    vec3 ambient = CalcAmbient(light);
    vec3 diffuse = CalcDiffuse(light, lightDir, normal);
    vec3 specular = CalcSpecular(light, lightDir, viewDir, normal);
    
    // combine and return
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    // properties
    vec3 lightDir = normalize(light.position - fragPos);
    float dist = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light, dist);

    // calculate
    vec3 ambient = CalcAmbient(light);
    vec3 diffuse = CalcDiffuse(light, lightDir, normal);
    vec3 specular = CalcSpecular(light, lightDir, viewDir, normal);

    // attenuate
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // combine and return
    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    // properties
    vec3 lightDir = normalize(light.position - fragPos);
    float dist = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light, dist);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerBeam - light.outerBeam;
    float intensity = clamp((theta - light.outerBeam) / epsilon, 0.0, 1.0);

    // calculate ambient now because we'll defo need it
    vec3 ambient = CalcAmbient(light);

    // init diffuse and spec as 0
    vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);
    // and update them if we're within the cone
    if (intensity > 0.0f)
    {
        diffuse = CalcDiffuse(light, lightDir, normal) * intensity * attenuation;
        specular = CalcSpecular(light, lightDir, viewDir, normal) * intensity * attenuation;
	}

    return (ambient + diffuse + specular);
}


float CalcAttenuation(Light light, float dist)
{
    // allegedly this should be 1 / dist if correcting for gamma
    // but this just turns all objects black for me, so...
    // leaving the function here in case I need it later though
    return 1.0 / (
        light.constant + 
        light.linear * dist + 
        light.quadratic * pow(dist, 2)
    );
}