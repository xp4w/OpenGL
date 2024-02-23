#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <stdio.h>

#include "defines.h"
#include "shaders.h"

/* Whenever the window size is changed this callback function executes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
/* Process all input */
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	}
}

int main()
{
	/* Initialize and configure GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create Window */
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test | OpenGL", NULL, NULL);
	if (window == NULL) { 
		printf("Failed to initialize OpenGL window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* Load all OpenGL function pointers */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	/* Vertex Shader */
	unsigned int vertexShader= glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/* Check shader for compile errors */
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Error when compiling shader: %s\n", infoLog);
	}

	/* Fragment shader */
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/* Check shader for compile errors */
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("Error when compiling shader: %s\n", infoLog);
	}

	/* Link shaders */
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	/* Check linker for compile errors */
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Error when compiling linker: %s\n", infoLog);
	}

	/* Clean up shader stuff */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Vertex data */
	float vertices[] = {
		/* X      Y     Z */
		 0.5f,  0.5f, 0.0f, /* Top right    */
		 0.5f, -0.5f, 0.0f, /* Bottom right */
		-0.5f, -0.5f, 0.0f, /* Bottom left  */
		-0.5f,  0.5f, 0.0f, /* Top left     */
	};
	unsigned int indices[] = {
		0, 1, 3, /* First triangle  */
		1, 2, 3, /* Second triangle */
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* Render loop */
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		/* Render background color */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw square ;) */
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Check and call events and swap buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* Clean up */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
