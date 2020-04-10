#include "Fluid.h"



Fluid::Fluid(int nParticles, int nCells, int containerWidth, int containerHeight) : nParticles(nParticles), containerWidth(containerWidth), containerHeight(containerHeight) {
	for (int i = 0; i < nParticles; i++) {
		positions.push_back(glm::vec3(i * 5, containerHeight / 2, 0.0f));
		velocities.push_back(glm::vec3(0.0f));
	}

	GLfloat vertices[] = { -1.0f, -1.0f, 0.0f,
						1.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 0.0f,
						-1.0f, 1.0f, 0.0f };

	GLuint indices[]{ 0, 1, 2,
					  2, 3, 0 };

	GLuint vbo, ibo;

	std::vector<glm::vec4> positions, velocities;
	std::vector<Cell> cells;

	for (int i = 0; i < nParticles; i++) {
		positions.push_back(glm::vec4(400.f, 300.f, 0.0f, 1.0f));
		velocities.push_back(glm::vec4(0.1f, 0.0f, 0.0f, 1.0f));
	}

	Cell* c = new Cell();
	for (int i = 0; i < nCells * nCells; i ++) {
		c->vel = glm::vec4(0.1f, 0.0f, 0.0f, 0.0f);
		cells.push_back(*c);
	}

	glGenBuffers(1, &positionBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nParticles * sizeof(glm::vec4), &positions[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionBO);

	glGenBuffers(1, &velocityBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nParticles * sizeof(glm::vec4), &velocities[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocityBO);

	glGenBuffers(1, &cellBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nParticles * sizeof(glm::vec4), &velocities[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BLOCK, 3, cellBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, positionBO);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	shader = new Shader("shader");
	shader->bind();
	shader->addUniform("nParticles");

	shader->setUniform1i("nParticles", nParticles);

	shader->bindCompute();
	GLuint computeID= shader->getComputeID();
	GLuint uniformLoc = glGetUniformLocation(computeID, "nCells");
	glUniform1i(uniformLoc, nCells);

	std::cout << "Create" << std::endl;
}

void Fluid::tick() {
	shader->bindCompute();
	glDispatchCompute(nParticles, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Fluid::render() {
	shader->bind();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Fluid::~Fluid() {
	glDeleteBuffers(1, &vao);
	delete shader;
}
