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
	int nParticles, nCells;
	int containerWidth, containerHeight;

	Shader *shader;

	GLuint vao, particleBO, velocityBO, cellBO;
	GLuint computeProgramID, computeProgramID2;
};

