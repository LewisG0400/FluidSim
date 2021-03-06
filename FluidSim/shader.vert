#version 440

layout (location = 0) in vec2 pos;

uniform mat4 transform;

out vec3 fragPos;

void main() {
	fragPos = vec3(transform * vec4(pos, 1.0));
	fragPos = pos;
	gl_Position = vec4(pos, 0.0, 1.0);
}