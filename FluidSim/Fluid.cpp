#include "Fluid.h"



Fluid::Fluid(int nParticles, int nCells, int containerWidth, int containerHeight) : nParticles(nParticles), nCells(nCells), containerWidth(containerWidth), containerHeight(containerHeight) {
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
		positions.push_back(glm::vec4(0.0f));
		velocities.push_back(glm::vec4(0.1f, 0.0f, 0.0f, 1.0f));
	}

	Cell* c = new Cell();
	c->vel = glm::vec4(0.1f, 0.1f, 0.0f, 1.0f);
	for (int i = 0; i < nCells * nCells; i ++) {
		cells.push_back(*c);
	}

	std::string computeSource = Shader::readFile("shader.compute");
	const GLchar* computeSourceA = computeSource.c_str();

	GLuint computeID, computeID2;
	GLint success;
	GLchar infoLog[512];

	computeID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeID, 1, &computeSourceA, NULL);
	glCompileShader(computeID);
	glGetShaderiv(computeID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(computeID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	computeID2 = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeID2, 1, &computeSourceA, NULL);
	glCompileShader(computeID2);
	glGetShaderiv(computeID2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(computeID2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	computeProgramID = glCreateProgram();
	glAttachShader(computeProgramID, computeID);
	glLinkProgram(computeProgramID);
	glGetProgramiv(computeProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(computeProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	computeProgramID2 = glCreateProgram();
	glAttachShader(computeProgramID2, computeID);
	glLinkProgram(computeProgramID2);
	glGetProgramiv(computeProgramID2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(computeProgramID2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(computeID);

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
	glBufferData(GL_SHADER_STORAGE_BUFFER, nCells * nCells * sizeof(Cell), &cells[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, cellBO);

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

	glUseProgram(computeProgramID);
	GLuint uniformLoc = glGetUniformLocation(computeProgramID, "nCells");
	glUniform1i(uniformLoc, nCells);

	glUseProgram(computeProgramID2);
	uniformLoc = glGetUniformLocation(computeProgramID2, "nCells");
	glUniform1i(uniformLoc, nCells);

	std::cout << "Create" << std::endl;
}

void Fluid::tick() {
	glUseProgram(computeProgramID2);
	glDispatchCompute(nCells, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(computeProgramID);
	glDispatchCompute(nParticles, 1, 1);
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
