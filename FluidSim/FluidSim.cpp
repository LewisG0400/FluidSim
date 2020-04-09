#define GLEW_STATIC

#include <iostream>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fluid Sim", NULL, NULL);
	glfwMakeContextCurrent(window);
}

int initGL() {
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		std::cin.get();
		return 1;
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void tick() {
	glfwPollEvents();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

void exit() {
	glfwDestroyWindow(window);
}

int main()
{
	initGLFW();
	std::cout << "Initialised GLFW" << std::endl;
	if(initGL() == 1) return 1;
	else std::cout << "Successfully initialised glew" << std::endl;

	while (!glfwWindowShouldClose(window)) {
		tick();
		render();
	}

	exit();
}
