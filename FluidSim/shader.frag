#version 430

uniform int nParticles;
uniform vec3 particles[512];

out vec4 colour;

void main() {
	colour = vec4(0.0f);

	for(int i = 0; i < nParticles; i ++) {
		float dist = distance(gl_FragCoord.xy, particles[i].xy);
		if(dist <= 5) colour += vec4(1.0f);
	}
}