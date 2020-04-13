#version 440
#extension GL_ARB_shader_storage_buffer_object : enable

layout (std140, binding = 1) buffer pos {
    vec4 positions[];
};

uniform int nParticles;

float ballRadius = 5.0f;
float thresh = 39.0f;

out vec4 colour;

void main() {
    colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    float total = 0.0f;

    for(int i = 0; i < nParticles; i ++) {
        total += pow(ballRadius, 2) / max(0.0001f, pow(length(vec2(positions[i].xy - gl_FragCoord.xy)), 1.0f));
    }

    if(total >= thresh) colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}