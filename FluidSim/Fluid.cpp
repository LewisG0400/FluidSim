#include "Fluid.h"



Fluid::Fluid(int nParticles, int containerWidth, int containerHeight) : nParticles(nParticles), containerWidth(containerWidth), containerHeight(containerHeight) {
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	glGenBuffers(1, &positionBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &positionBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

	glm::vec3 *positions = (glm::vec3 *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, nParticles, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < nParticles; i++) {
		positions[i] = glm::vec3(0.0f);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glGenBuffers(1, &velocityBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

	glm::vec3 *positions = (glm::vec3 *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, nParticles, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < nParticles; i++) {
		positions[i] = glm::vec3(0.0f);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	shader = new Shader("shader");
	shader->bind();
	shader->addUniform("nParticles");
	shader->addUniform("particles");

	shader->setUniform1i("nParticles", nParticles);
	shader->setUniformVec3Arrayf("particles", &positions[0], nParticles);

	std::cout << "Create" << std::endl;
}

void Fluid::tick() {
	for (int i = 0; i < nParticles; i++) {
		positions.at(i) += velocities.at(i);
	}

	shader->setUniformVec3Arrayf("particles", &positions[0], nParticles);
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
