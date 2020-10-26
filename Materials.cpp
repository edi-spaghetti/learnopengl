#include "Materials.h"

Material::Material()
{
	// default constructer when materials are declared but not initialised
	ambient = glm::vec3(1.0f);
	diffuse = glm::vec3(1.0f);
	specular = glm::vec3(1.0f);
	shininess = 32.0f;
}

Material::Material(float amb, float diff, float spec, float shine)
{
	ambient = glm::vec3(amb);
	diffuse = glm::vec3(diff);
	specular = glm::vec3(spec);
	shininess = shine;
}

Material::Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shine)
{
	ambient = amb;
	diffuse = diff;
	specular = spec;
	shininess = shine;
}