#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm/glm.hpp>


class Material
{
public:
    Material();
    Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shine);

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class MaterialManager
{
public:
    Material emerald = Material(
        glm::vec3(0.0215f, 0.1745f, 0.0215f),
        glm::vec3(0.07568f, 0.61424f, 0.07568f),
        glm::vec3(0.633f, 0.727811f, 0.633f),
        0.6f
    );
    Material jade = Material(
        glm::vec3(0.135f, 0.2225f, 0.1575f),
        glm::vec3(0.54f, 0.89f, 0.63f),
        glm::vec3(0.316228f, 0.316228f, 0.316228f),
        0.1f
    );
    Material obsidian = Material(
        glm::vec3(0.05375f, 0.05f, 0.06625f),
        glm::vec3(0.18275f, 0.17f, 0.22525f),
        glm::vec3(0.332741f, 0.328634f, 0.346435f),
        0.3f
    );
    Material pearl = Material(
        glm::vec3(0.25f, 0.20725f, 0.20725f),
        glm::vec3(1.0f, 0.829f, 0.829f),
        glm::vec3(0.296648f, 0.296648f, 0.296648f),
        0.088f
    );
    Material ruby = Material(
        glm::vec3(0.1745f, 0.01175f, 0.01175f),
        glm::vec3(0.61424f, 0.04136f, 0.04136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        0.6f
    );
    Material turquoise = Material(
        glm::vec3(0.1f, 0.18725f, 0.1745f),
        glm::vec3(0.396f, 0.74151f, 0.69102f),
        glm::vec3(0.297254f, 0.30829f, 0.306678f),
        0.1f
    );
    Material brass = Material(
        glm::vec3(0.329412f, 0.223529f, 0.027451f),
        glm::vec3(0.780392f, 0.568627f, 0.113725f),
        glm::vec3(0.992157f, 0.941176f, 0.807843f),
        0.21794872f
    );
    Material bronze = Material(
        glm::vec3(0.2125f, 0.1275f, 0.054f),
        glm::vec3(0.714f, 0.4284f, 0.18144f),
        glm::vec3(0.393548f, 0.271906f, 0.166721f),
        0.2f
    );
    Material chrome = Material(
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.774597f, 0.774597f, 0.774597f),
        0.6f
    );
    Material copper = Material(
        glm::vec3(0.19125f, 0.0735f, 0.0225f),
        glm::vec3(0.7038f, 0.27048f, 0.0828f),
        glm::vec3(0.256777f, 0.137622f, 0.086014f),
        0.1f
    );
    Material gold = Material(
        glm::vec3(0.24725f, 0.1995f, 0.0745f),
        glm::vec3(0.75164f, 0.60648f, 0.22648f),
        glm::vec3(0.628281f, 0.555802f, 0.366065f),
        0.4f
    );
    Material silver = Material(
        glm::vec3(0.19225f, 0.19225f, 0.19225f),
        glm::vec3(0.50754f, 0.50754f, 0.50754f),
        glm::vec3(0.508273f, 0.508273f, 0.508273f),
        0.4f
    );
    Material black_plastic = Material(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.01f, 0.01f, 0.01f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        0.25f
    );
    Material cyan_plastic = Material(
        glm::vec3(0.0f, 0.1f, 0.06f),
        glm::vec3(0.0f, 0.50980392f, 0.50980392f),
        glm::vec3(0.50196078f, 0.50196078f, 0.50196078f),
        0.25f
    );
    Material green_plastic = Material(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.1f, 0.35f, 0.1f),
        glm::vec3(0.45f, 0.55f, 0.45f),
        0.25f
    );
    Material red_plastic = Material(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.0f, 0.0f),
        glm::vec3(0.7f, 0.6f, 0.6f),
        0.25f
    );
    Material white_plastic = Material(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.55f, 0.55f, 0.55f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        0.25f
    );
    Material yellow_plastic = Material(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.6f, 0.6f, 0.5f),
        0.25f
    );
    Material black_rubber = Material(
        glm::vec3(0.02f, 0.02f, 0.02f),
        glm::vec3(0.01f, 0.01f, 0.01f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        0.078125f
    );
    Material cyan_rubber = Material(
        glm::vec3(0.0f, 0.05f, 0.05f),
        glm::vec3(0.4f, 0.5f, 0.5f),
        glm::vec3(0.04f, 0.7f, 0.7f),
        0.078125f
    );
    Material green_rubber = Material(
        glm::vec3(0.0f, 0.05f, 0.0f),
        glm::vec3(0.4f, 0.5f, 0.4f),
        glm::vec3(0.04f, 0.7f, 0.04f),
        0.078125f
    );
    Material red_rubber = Material(
        glm::vec3(0.05f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.4f, 0.4f),
        glm::vec3(0.7f, 0.04f, 0.04f),
        0.078125f
    );
    Material white_rubber = Material(
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        0.078125f
    );
    Material yellow_rubber = Material(
        glm::vec3(0.05f, 0.05f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.4f),
        glm::vec3(0.7f, 0.7f, 0.04f),
        0.078125f
    );
};

#endif MATERIALS_H