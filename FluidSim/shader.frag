#version 440

uniform int nParticles;

layout(std140, binding=1) buffer pos {
	vec3 positions[];
};

out vec4 colour;

void main() {
	colour = vec4(0.0f);

	for(int i = 0; i < nParticles; i ++) {
		float dist = distance(gl_FragCoord.xy, positions[i].xy);
		if(dist <= 5) colour += vec4(1.0f);
	}
}