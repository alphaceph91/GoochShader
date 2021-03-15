#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "soil.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h> //very important shader class
#include <learnopengl/camera.h>
#include <learnopengl/model.h> //very important shader class

using namespace glm;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

#define NUM_VERTICES 36

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

//SCREEN SETTINGS
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//CAMERA ATTRIBUTES
vec3 cameraPos = vec3(0.0f, 1.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

//light attributes
vec3 lightPos = vec3(1.0f, 1.0f, 2.0f); //can be changed here for different result

//GOOCH ATTRIBUTES-MENTION IN THE REPORT-these values can be changed for different gooch results
vec3 SurfaceColor = vec3(0.0, 0.0, 0.0);
vec3 WarmColor = vec3(0.5, 0.5, 0.0);
vec3 CoolColor = vec3(0.0, 0.0, 0.8);
float DiffuseWarm = 0.45;
float DiffuseCool = 0.45;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

int currentShader = 0;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW Window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GoochToon", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader goochShader("goochShader.vert", "goochShader.frag");
	Shader toonShader("toonShader.vert","toonShader.frag");
	Model myModel("objects/happy-buddha/happy-buddha.obj");

	glEnable(GL_DEPTH_TEST);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
	
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		mat4 projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
		mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 model = mat4(1.0f);  
		model = rotate(model, (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f));
		//model = rotate(model, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f)); //different rotation
		//model = translate(model, vec3(0.0f, -0.5f, 0.0f)); //gives another model in toon. idk why
		model = scale(model, vec3(0.2f, 0.2f, 0.2f));

		//Accessing strings and funtions from the shader class

		//gooch shader
		goochShader.setMat4("projection", projection); //setMat4 is used for projection matrix
		goochShader.setMat4("view", view);
		goochShader.setMat4("model", model);
		goochShader.setVec3("SurfaceColor", SurfaceColor); //setvec3 is used for SurfaceColor, a 3-component vector 
		goochShader.setVec3("WarmColor", WarmColor);
		goochShader.setVec3("CoolColor", CoolColor);
		goochShader.setFloat("DiffuseWarm", DiffuseWarm); //setFloat is used for DiffuseWarm, a float value
		goochShader.setFloat("DiffuseCool", DiffuseCool);
		goochShader.setVec3("lightPos", lightPos);

		myModel.Draw(goochShader);

		//toon shader
		toonShader.setMat4("projection", projection);
		toonShader.setMat4("view", view);
		toonShader.setMat4("model", model);
		toonShader.setVec3("lightPos", lightPos);

		//switch-MENTION IN THE REPORT
		if (currentShader == 0)
		{
			myModel.Draw(goochShader);
			goochShader.use();
		}
		else
		{
			myModel.Draw(toonShader);
			toonShader.use();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//switch between shaders
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		currentShader = 0;

	}	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		currentShader = 1;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}