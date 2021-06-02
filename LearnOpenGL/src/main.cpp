#include <glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "filesystem.h"
#include "private.h" //for system files and stuff, comments where this is used and will need replaced with your own stuff

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
glm::mat3 mat4tomat3(glm::mat4 fourMatrix);
unsigned int loadTexture(char const* path);

const unsigned int SCR_WIDTH = 920;
const unsigned int SCR_HEIGHT = 690;

float mixValue = 0.0f;

//stuff to keep movement speed constant across different hardware
float deltaTime = 0.0f; //time between last and current frames
float lastFrame = 0.0f;

//camera stuff
Camera camera(glm::vec3(5.0f, 5.0f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool axesOn = true;

int main() {
	glfwInit(); //configures GLFW for OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //Creates pointer to window object
	if (window == NULL) //if window creation fails
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1; //main function should return 0 if successfull
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //initializes GLAD so we can call those functions
	{
		std::cout << "Failed to initialize GLAD you boomer :(" << std::endl;
		return -1;
	}

	srand((unsigned int)time(NULL));

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //first two parameters set the bottom left corner of the screen's coordinates

	//enables us to use alpha channel goodness
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); 

	Shader cubeShader("shaders/cube.vert", "shaders/cube.frag");
	Shader lightShader("shaders/cube.vert", "shaders/light.frag");
	Shader axesShader("shaders/axes.vert", "shaders/axes.frag");


	//c u b e
	float cube[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(cubeVAO);

	//sets vertex position attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//texturing
	//unsigned int diffuseMap = loadTexture(FileSystem::getPath("blue.png").c_str()); not working :(
	//unsigned int specularMap = loadTexture(FileSystem::getPath("invert.png").c_str());


	//paths are in private file for now, can't get FileSystem class to work just yet. Replace with your own paths
	unsigned int diffuseMap = loadTexture(diffuseMapPath.c_str());
	unsigned int specularMap = loadTexture(specularMapPath.c_str());
	unsigned int emissionMap = loadTexture(emissionMapPath.c_str());

	cubeShader.use();
	cubeShader.setInt("material.diffuse", 0);
	cubeShader.setInt("material.specular", 1);
	cubeShader.setInt("material.emission", 2);

	//light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//axes
	float endpoint = 1000.0f;
	float axes[]{
		//vertices			//colors
		endpoint, 0, 0,		1.0f, 0, 0,
		0, 0, 0,			1.0f, 0, 0,
		-endpoint, 0, 0,	0.5f, 0, 0,
		0, 0, 0,			0.5f, 0, 0,
		0, endpoint, 0,		0, 1.0f, 0,
		0, 0, 0,			0, 1.0f, 0,
		0, -endpoint, 0,	0, 0.5f, 0,
		0, 0, 0,			0, 0.5f, 0,
		0, 0, endpoint,		0, 0, 1.0f,
		0, 0, 0,			0, 0, 1.0f,
		0, 0, -endpoint,	0, 0, 0.5f,
		0, 0, 0,			0, 0, 0.5f
	};

	unsigned int axesVBO, axesVAO;
	glGenVertexArrays(1, &axesVAO);
	glGenBuffers(1, &axesVBO);

	glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	glBindVertexArray(axesVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//generate a whole bunch of cube bois, might wanna make it an object 
	const unsigned int numCubes = 25;
	float posLow = 0.0f;
	float posHigh = 10.0f;
	glm::vec3 cubePos[numCubes];
	for (int i = 0; i < numCubes; i++) 
	{
		float cubex = -posHigh + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * posHigh - posLow)));
		float cubey = -posHigh + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * posHigh - posLow)));
		float cubez = posLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (posHigh - posLow)));
		cubePos[i] = glm::vec3(cubex, cubey, cubez);
	}
	glm::vec3 cubeCol[numCubes];
	for (int i = 0; i < numCubes; i++) 
	{
		float oColorR = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / -0.5f));
		float oColorG = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / -0.5f));
		float oColorB = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / -0.5f));
		cubeCol[i] = glm::vec3(oColorR, oColorG, oColorB);
	}

	float scaleLow = 0.75f;
	float scaleHigh = 1.25f;
	glm::vec3 cubeScale[numCubes];
	for (int i = 0; i < numCubes; i++)
	{
		float scalex = scaleLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (scaleHigh - scaleLow)));
		float scaley = scaleLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (scaleHigh - scaleLow)));
		float scalez = scaleLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (scaleHigh - scaleLow)));
		cubeScale[i] = glm::vec3(scalex, scaley, scalez);
	}
	float cubeSpeed[numCubes];
	for (int i = 0; i < numCubes; i++)
	{
		cubeSpeed[i] = scaleLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (scaleHigh - scaleLow)));
	}


	//light settings
	float lightRadius = 5.0f;
	float lightSpeed = 0.5f;

	float shininess = 32.0f;
	float ambientStrength = 0.02f;
	float diffuseStrength = 0.5f;
	float specularStrength = 0.75f;

	while (!glfwWindowShouldClose(window))//glfwWindowShouldClose checks if it's been instructed to close
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glm::vec3 worldColor = glm::vec3(0.07, 0.07, 0.07);
		glm::vec3 worldColorMixed = (glm::vec3(1.0, 1.0, 1.0f) - glm::vec3(mixValue, mixValue, mixValue)) * worldColor + glm::vec3(0.01, 0.01, 0.01);
		glClearColor(worldColorMixed.x, worldColorMixed.y, worldColorMixed.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//w = 0 ==> direction vector, w = 1 ==> position vector
		glm::vec4 lightInfo = glm::vec4(lightRadius * sin(glfwGetTime() * lightSpeed), lightRadius * cos(glfwGetTime()* lightSpeed), 3.0 * sin(glfwGetTime() * lightSpeed * 1.7) + 5.0, 1.0);
		glm::vec3 lightColor = glm::vec3(1.0f - mixValue);

		cubeShader.use();
		cubeShader.setFloat("mixValue", mixValue);
		//cubeShader.setVec3("lightColor", lightColor);
		//cubeShader.setVec3("lightPos", lightPos);
		cubeShader.setVec3("viewPos", camera.position);
		//cubeShader.setFloat("shininess", shininess);

		/*cubeShader.setFloat("material.shininess", shininess);
		cubeShader.setVec3("material.ambient", cubeCol[0]);
		cubeShader.setVec3("material.diffuse", cubeCol[0]);
		cubeShader.setVec3("material.specular", cubeCol[0]);*/

		cubeShader.setVec4("light.vector", lightInfo);
		glm::vec3 lightAmbient = ambientStrength * lightColor;
		cubeShader.setVec3("light.ambient", lightAmbient);
		glm::vec3 lightDiffuse = diffuseStrength * lightColor;
		cubeShader.setVec3("light.diffuse", lightDiffuse);
		cubeShader.setVec3("light.specular", lightColor);

		cubeShader.setFloat("light.a", 0.032f);
		cubeShader.setFloat("light.b", 0.09f);
		cubeShader.setFloat("light.c", 1.0f);

		//Vertices -> screen 3D goodness
		glm::mat4 projection = glm::perspective(camera.zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		cubeShader.setMat4("projection", projection);

		glm::mat4 view = camera.getViewMatrix();
		cubeShader.setMat4("view", view);

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeShader.setMat4("model", cubeModel);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		glBindVertexArray(cubeVAO);

		for (unsigned int i = 0; i < numCubes; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePos[i]);
			model = glm::scale(model, cubeScale[i]);
			float angle = 1.1f * i;
			model = glm::rotate(model, (float)glfwGetTime() * cubeSpeed[i] + angle, glm::vec3(0.5f, 1.0f, 0.0f));
			cubeShader.setMat4("model", model);
			//cubeShader.setVec3("objectColor", cubeCol[i]);

			glm::mat3 normalMatrix = glm::transpose(glm::inverse(mat4tomat3(model)));
			cubeShader.setMat3("normalMatrix", normalMatrix);

			cubeShader.setFloat("material.shininess", shininess);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//light cube
		lightShader.use();
		lightShader.setVec3("lightColor", lightColor);
		lightShader.setFloat("mixValue", mixValue);
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, glm::vec3(lightInfo.x, lightInfo.y, lightInfo.z));
		lightShader.setMat4("model", lightModel);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//axes
		if (axesOn)
		{
			axesShader.use();
			//glm::mat4 view = camera.getViewMatrix();
			axesShader.setMat4("view", view);
			//glm::mat4 projection = glm::perspective(camera.zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			axesShader.setMat4("projection", projection);
			glm::mat4 axesModel = glm::mat4(1.0f);
			axesShader.setMat4("model", axesModel);

			glBindVertexArray(axesVAO);
			glLineWidth(3.0f);
			glDrawArrays(GL_LINES, 0, 12);
		}

		glfwSwapBuffers(window); //swaps the color buffer (kinda reloads the screen)
		glfwPollEvents(); //checks if any events are triggered
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &axesVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &axesVBO);

	glfwTerminate(); //cleans up resources and stuff properly
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) //function that's called whenever we resize the window
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	//close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if escape key is pressed, the window is closed
	{
		glfwSetWindowShouldClose(window, true);
	}
	//transparency stuff (choose different functionality for arrow keys later)
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.01;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.01f; 
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
	//camera movement (WASD)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//{
	//	//camUp.y += camSpeed;
	//}
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	//{
	//	//camUp.y -= camSpeed;
	//}
}
void scroll_callback(GLFWwindow*, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
	{
		axesOn = !axesOn;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
	{
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}
//should probably move elsewhere
//obtains upper left 3x3 matrix of a 4x4 matrix so we don't have to calculate the normal matrix for each vertex
glm::mat3 mat4tomat3(glm::mat4 fourMat)
{
	float threeVals[9];
	const float* pSource = (const float*)glm::value_ptr(fourMat);
	unsigned int j = 0;
	for (unsigned int i = 0; i < 9; i++)
	{
		if ((j + 1) % 4 == 0)
		{
			j++;
		}
		threeVals[i] = pSource[j];
		j++;
	}
	glm::mat3 newMat = glm::make_mat3(threeVals);
	return newMat;
}
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			format = GL_RED;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}
