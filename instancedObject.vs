#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 model;
// locations 4, 5 and 6 are also used by model attribute
layout (location = 7) in mat3 normalMatrix;

uniform mat4 view;
uniform mat4 projection;

out IO_BLOCK {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} vs_out;

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = normalMatrix * aNormal;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
