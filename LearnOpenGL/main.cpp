#include <glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n" //Simple C Vertex shader program
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n" //Simple C Fragment Shader Program
	"out vec4 FragColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n" //right now this is a constant color
	"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height) //function that's called whenever we resize the window
{
	glViewport(0, 0, width, height); 
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if escape key is pressed, the window is closed
		glfwSetWindowShouldClose(window, true);
}


int main() {
	glfwInit(); //configures GLFW for OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); //Creates pointer to window object
	if (window == NULL) //if window creation fails
	{
		std::cout << "Failed to create GLFW window you manic donut :(" << std::endl;
		glfwTerminate();
		return -1; //main function should return 0 if successfull
	}
	glfwMakeContextCurrent(window);



	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //initializes GLAD so we can call those functions
	{
		std::cout << "Failed to initialize GLAD you boomer :(" << std::endl;
		return -1;
	}




	unsigned int vertexShader; //object for vertex data (like position)
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //compiles the C program to our vertex shader object
	glCompileShader(vertexShader);

	//checks to see if our vertex shader compiled good
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	unsigned int fragmentShader; //object that assigns color data to vertices
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //compiles the C program to our fragment shader object
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	unsigned int shaderProgram; //object that links the shaders, we won't need them afterwards
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glViewport(0, 0, 800, 600); //first two parameters set the bottom left corner of the screen's coordinates

	float vertices[] = {
	 0.5f,  0.5f, 0.0f, //0
	 0.5f, -0.5f, 0.0f, //1
	-0.5f, -0.5f, 0.0f, //2
	-0.5f,  0.5f, 0.0f  //3
	};
	unsigned int indices[] = {
		0, 1, 3, //first triangle
		//1, 2, 3 //second triangle
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	unsigned int VBO; //Vertex Buffer Object: Sends many vertices to the GPU at once rather than one at a time, increasing efficiency
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Makes it so that whenever we make a bueffer call on the GL_ARRAY_BUFFER target it'll use VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*
	*Copies data from vertices (data size=sizeof(vertices)) into the GL_ARRAY_BUFFER target. As for the fourth parameter
		GL_STREAM_DRAW: the data is set only once and is only used by the GPU a few times at most
		GL_STATIC_DRAW: the data is set only once and is used many times
		GL_DYNAMIC_DRAW: the data is changed a lot and is used many times
	*Here the triangle's position data doesn't change, but we want it to keep rendering, so we'll use the static option.
	*If it were to change a lot, we'd use dynamic, which puts the data in memory 
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/*
	* 1. specifies vertex attribute you wanna use. Here we're using location, which we set to 0 when setting up our vertex shader
	* 2. is the size of the vertex attribute, which is 3 for vec3
	* 3. We're using floating point values
	* 4. Whether data is "normalized" to 0 or not
	* 5. The "stride", basically the spacing between consecutive vertex attributes
	* 6. The "offset" of where teh position data begins in the buffer
	*/
	glEnableVertexAttribArray(0);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //bind vertex array object
	//copies vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//sets vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	while (!glfwWindowShouldClose(window))//glfwWindowShouldClose checks if it's been instructed to close
	{
		processInput(window); //checks for inputs

		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //clears the color buffer and resets it to the above value
		
		glUseProgram(shaderProgram);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		glfwSwapBuffers(window); //swaps the color buffer (kinda reloads the screen)
		glfwPollEvents(); //checks if any events are triggered
	}


	glfwTerminate(); //cleans up resources and stuff properly
	return 0;
}