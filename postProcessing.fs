#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool gammaCorrection = true;
const float offset = 1.0 / 300.0;
const float gamma = 2.2;


void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2(      0,  offset),
        vec2( offset,  offset),
        vec2(-offset,       0),
        vec2(      0,       0),
        vec2( offset,       0),
        vec2(-offset, -offset),
        vec2(      0, -offset),
        vec2( offset, -offset)
    );

    float[9] none = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    float[9] sharpen = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    float[9] blur = {1.0/16, 2.0/16, 1.0/16, 2.0/16, 4.0/16, 2.0/16, 1.0/16, 2.0/16, 1.0/16};
    float[9] edges = {1, 1, 1, 1, -8, 1, 1, 1, 1};

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 colour = vec3(0);
    for (int i = 0; i < 9; i++)
    {
        colour += sampleTex[i] * none[i];
    }

    FragColor = vec4(colour, 1);
    if (gammaCorrection)
    {
        // add gamma correction
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
    }
}
