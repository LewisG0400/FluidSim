#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GLEW/glew.h>

#include "Shader.h"

typedef struct cell {
	glm::vec4 vel;

} Cell;

class Fluid
{
public:
	Fluid(int nParticles, int nCells, int containerWidth, int containerHeight);

	void tick();
	void render();

	~Fluid();

private:
	int nParticles;
	int containerWidth, containerHeight;
	std::vector<glm::vec3> positions, velocities;

	Shader *shader;

	GLuint vao, positionBO, velocityBO, cellBO;
};

