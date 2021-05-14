#include <glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {
	glfwInit(); //configures GLFW for OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //Creates pointer to window object
	if (window == NULL) //if window creation fails
	{
		std::cout << "Failed to create GLFW window you manic donut :(" << std::endl;
		glfwTerminate();
		return -1; //main function should return 0 if successfull
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //initializes GLAD so we can call those functions
	{
		std::cout << "Failed to initialize GLAD you boomer :(" << std::endl;
		return -1;
	}

	Shader ourShader("shader.vs", "shader.fs");


	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //first two parameters set the bottom left corner of the screen's coordinates

	float vertices[] = {
	//positions			 //colors
	 -0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, //top right
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, //bottom right
	 0.0f, 0.731f, 0.0f,  0.0f, 0.5f, 1.0f //center
	};


	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//sets vertex position attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*
	* 1. specifies vertex attribute you wanna use. Here we're using location, which we set to 0 when setting up our vertex shader
	* 2. is the size of the vertex attribute, which is 3 for vec3
	* 3. We're using floating point values
	* 4. Whether data is "normalized" to 0 or not
	* 5. The "stride", basically the spacing between consecutive vertex attributes
	* 6. The "offset" of where the position data begins in the buffer
	*/

	//sets vertex color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//glUseProgram(shaderProgram);


	while (!glfwWindowShouldClose(window))//glfwWindowShouldClose checks if it's been instructed to close
	{
		ourShader.use();
		ourShader.setFloat("someUniform", 1.0f);
		processInput(window); //checks for inputs

		glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //clears the color buffer and resets it to the above value

		/*float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, greenValue, greenValue, 0.0f, 1.0f);*/

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		glfwSwapBuffers(window); //swaps the color buffer (kinda reloads the screen)
		glfwPollEvents(); //checks if any events are triggered
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate(); //cleans up resources and stuff properly
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) //function that's called whenever we resize the window
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if escape key is pressed, the window is closed
		glfwSetWindowShouldClose(window, true);
}