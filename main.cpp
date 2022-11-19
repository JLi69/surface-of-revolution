#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_util.h"
#include <chrono>
#include <iostream>

#define DEFAULT_WIDTH 900
#define DEFAULT_HEIGHT 600

#define SPEED 8.0f

//Perspective matrix
static glm::mat4 persp = glm::perspective(75.0f, (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT, 0.1f, 1000.0f);
//Model view matrix
static glm::mat4 modelViewMat = glm::mat4(1.0f);
//Camera position
static glm::vec3 camPos = glm::vec3(0.0f, -1.0f, 20.0f);
static glm::vec3 camMovement = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 camRot = glm::vec3(0.0f, 0.0f, 0.0f); //Rotation about axes stored as radians
static glm::vec3 camRotSpeed = glm::vec3(0.0f, 0.0f, 0.0f);

void handleWinResize(GLFWwindow *win, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	persp = glm::perspective(75.0f, (float)newWidth / (float)newHeight, 0.1f, 1000.0f);
}

void handleKeyInput(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	static std::map<int, bool> pressed;

	//Move forward	
	if(action == GLFW_PRESS)
	{
		switch(key)
		{
		//W overrides S
		case GLFW_KEY_W:
			pressed[GLFW_KEY_S] = false;
			break;	
		//S overrides W
		case GLFW_KEY_S:		
			pressed[GLFW_KEY_W] = false;
			break;
		//A overrides D
		case GLFW_KEY_A:
			pressed[GLFW_KEY_A] = false;
			break;	
		//D overrides A
		case GLFW_KEY_D:		
			pressed[GLFW_KEY_D] = false;
			break;
		//Space overrides Shift
		case GLFW_KEY_SPACE:	
			pressed[GLFW_KEY_LEFT_SHIFT] = false;
			pressed[GLFW_KEY_RIGHT_SHIFT] = false;
			break;
		//Shift overrides Space
		case GLFW_KEY_LEFT_SHIFT: //Falls through
		case GLFW_KEY_RIGHT_SHIFT:
			pressed[GLFW_KEY_SPACE] = false;
			break;
		default: break;	
		}

		pressed[key] = true;	
	}	
	else if(action == GLFW_RELEASE)
		pressed[key] = false;

	//Set camera speed
	if(pressed[GLFW_KEY_W]) camMovement.z = -SPEED;
	else if(pressed[GLFW_KEY_S]) camMovement.z = SPEED;
	else camMovement.z = 0.0f;

	if(pressed[GLFW_KEY_A]) camMovement.x = SPEED;
	else if(pressed[GLFW_KEY_D]) camMovement.x = -SPEED;
	else camMovement.x = 0.0f;

	if(pressed[GLFW_KEY_SPACE]) camMovement.y = -SPEED;	
	else if(pressed[GLFW_KEY_LEFT_SHIFT] || pressed[GLFW_KEY_RIGHT_SHIFT]) camMovement.y = SPEED;
	else camMovement.y = 0.0f;
}

void updateCamera(float secondsPerFrame)
{
	//Move camera
	camPos += camMovement * secondsPerFrame;
}

int main()
{
	//Initailize window
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Surface of Revolution", NULL, NULL);
	glfwMakeContextCurrent(win);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetWindowSizeCallback(win, handleWinResize);
	glfwSetKeyCallback(win, handleKeyInput);

	//Create cylinder
	GLUtil::BufferValues cylinderObj = GLUtil::generateCylinder(48);
	GLUtil::Buffers cylinderBuffers = GLUtil::genVertBuffers(cylinderObj);
	GLUtil::bindBuffers(cylinderBuffers);

	//Create shader program
	GLUtil::ShaderProgram program;
	program.id = GLUtil::createShaderProgram("res/shaders/3d.vert", "res/shaders/rainbow.frag");
	program.uniforms["uPerspMat"] = glGetUniformLocation(program.id, "uPerspMat");
	program.uniforms["uMVMat"] = glGetUniformLocation(program.id, "uMVMat");	
	glUseProgram(program.id);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	

	std::chrono::time_point<std::chrono::system_clock> start, end;		
	static float secondsPerFrame = 1.0f;
	while(!glfwWindowShouldClose(win))
	{
		start = std::chrono::system_clock::now();

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		modelViewMat = glm::translate(glm::mat4(1.0f), -camPos);
		glUniformMatrix4fv(program.uniforms["uPerspMat"], 1, false, glm::value_ptr(persp));	
		glUniformMatrix4fv(program.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
		glDrawArraysInstanced(GL_TRIANGLES, 0, cylinderObj.verts.size(), 2560);
	
		updateCamera(secondsPerFrame);

		GLUtil::outputGLErrors();
		glfwPollEvents();
		glfwSwapBuffers(win);

		end = std::chrono::system_clock::now();
		std::chrono::duration<float> passed = end - start;
		float seconds = passed.count();
		secondsPerFrame = seconds;
		std::cout << "Frames Per Second: " << 1.0f / seconds << '\n';
	}
	glfwTerminate();
}
