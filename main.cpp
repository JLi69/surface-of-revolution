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
#define ROTATION_SPEED 0.1f

#define ANIMATION_SPEED 1.0f

enum
{
	NORMAL,
	FLAT,
	ANIMATION
};

//Perspective matrix
static glm::mat4 persp = glm::perspective(75.0f, (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT, 0.1f, 1000.0f);
//Model view matrix
static glm::mat4 modelViewMat = glm::mat4(1.0f);
//Camera position
static glm::vec3 camPos = glm::vec3(0.0f, -1.0f, 20.0f);
static glm::vec3 camMovement = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 camRot = glm::vec3(0.0f, 0.0f, 0.0f); //Rotation about axes stored as radians
static glm::vec3 camRotSpeed = glm::vec3(0.0f, 0.0f, 0.0f);
static int viewMode = NORMAL;
static float maxAngle = 999.0f;

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
			pressed[GLFW_KEY_A] = false;
			pressed[GLFW_KEY_D] = false;
			break;	
		//S overrides W
		case GLFW_KEY_S:		
			pressed[GLFW_KEY_W] = false;
			pressed[GLFW_KEY_A] = false;
			pressed[GLFW_KEY_D] = false;	
			break;
		//A overrides D
		case GLFW_KEY_A:
			pressed[GLFW_KEY_A] = false;
			pressed[GLFW_KEY_W] = false;
			pressed[GLFW_KEY_S] = false;	
			break;	
		//D overrides A
		case GLFW_KEY_D:		
			pressed[GLFW_KEY_A] = false;
			pressed[GLFW_KEY_W] = false;
			pressed[GLFW_KEY_S] = false;	
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
		//Release/Recapture mouse cursor
		case GLFW_KEY_ESCAPE:
			glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ?
				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL) :
				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
			camRotSpeed = glm::vec3(0.0f, 0.0f, 0.0f);	
			break;		
		//Switch mode
		case GLFW_KEY_1:
			viewMode = NORMAL;	
			maxAngle = 999.0f;
			break;
		case GLFW_KEY_2:
			viewMode = FLAT;	
			maxAngle = 999.0f;
			break;	
		case GLFW_KEY_3:
			viewMode = ANIMATION;
			maxAngle = 0.0f;
			break;	
		default: break;	
		}

		pressed[key] = true;	
	}	
	else if(action == GLFW_RELEASE)
		pressed[key] = false;

	//Set camera speed
	if(pressed[GLFW_KEY_W]) 
	{	
		camMovement.z = -SPEED * cosf(-camRot.y);
		camMovement.x = -SPEED * sinf(-camRot.y);
	}	
	else if(pressed[GLFW_KEY_S]) 
	{
		camMovement.z = SPEED * cosf(-camRot.y);
		camMovement.x = SPEED * sinf(-camRot.y);
	}
	else if(pressed[GLFW_KEY_A]) 
	{	
		camMovement.z = SPEED * cosf(-camRot.y + PI / 2.0f);
		camMovement.x = SPEED * sinf(-camRot.y + PI / 2.0f);
	}	
	else if(pressed[GLFW_KEY_D]) 
	{
		camMovement.z = SPEED * cosf(-camRot.y - PI / 2.0f);
		camMovement.x = SPEED * sinf(-camRot.y - PI / 2.0f);
	}
	else
	{
		camMovement.z = 0.0f;
		camMovement.x = 0.0f;
	}

	if(pressed[GLFW_KEY_SPACE]) camMovement.y = -SPEED;	
	else if(pressed[GLFW_KEY_LEFT_SHIFT] || pressed[GLFW_KEY_RIGHT_SHIFT]) camMovement.y = SPEED;
	else camMovement.y = 0.0f;
}

void cursorPositionCallback(GLFWwindow* win, double x, double y)
{
	static double xpos = x, ypos = y;	

	if(glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		camRotSpeed.y = -(x - xpos) * ROTATION_SPEED;
		camRotSpeed.x = -(y - ypos) * ROTATION_SPEED;
	}
		
	xpos = x;
	ypos = y;
}

void updateCamera(float secondsPerFrame)
{
	//Move camera
	camPos += camMovement * secondsPerFrame;
	//Rotate camera
	camRot += camRotSpeed * secondsPerFrame;

	if(camRot.x > PI / 2.0f)
		camRot.x = PI / 2.0f;
	else if(camRot.x < -PI / 2.0f)
		camRot.x = -PI / 2.0f;
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

	//Grab and hide cursor
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Create cylinder
	GLUtil::BufferValues cylinderObj = GLUtil::generateCylinder(48);
	GLUtil::Buffers cylinderBuffers = GLUtil::genVertBuffers(cylinderObj);

	GLUtil::BufferValues lineObj = GLUtil::generateLine();
	GLUtil::Buffers lineBuffers = GLUtil::genVertBuffers(lineObj);

	//Create shader program
	GLUtil::ShaderProgram program;
	program.id = GLUtil::createShaderProgram("res/shaders/3d.vert", "res/shaders/rainbow.frag");
	program.uniforms["uPerspMat"] = glGetUniformLocation(program.id, "uPerspMat");
	program.uniforms["uMVMat"] = glGetUniformLocation(program.id, "uMVMat");	
	program.uniforms["uMaxAngle"] = glGetUniformLocation(program.id, "uMaxAngle");

	GLUtil::ShaderProgram simple3d;
	simple3d.id = GLUtil::createShaderProgram("res/shaders/simple3d.vert", "res/shaders/simple.frag");
	simple3d.uniforms["uPerspMat"] = glGetUniformLocation(simple3d.id, "uPerspMat");
	simple3d.uniforms["uMVMat"] = glGetUniformLocation(simple3d.id, "uMVMat");	
	simple3d.uniforms["uCol"] = glGetUniformLocation(simple3d.id, "uCol");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::chrono::time_point<std::chrono::system_clock> start, end;		
	static float secondsPerFrame = 1.0f;
	while(!glfwWindowShouldClose(win))
	{
		start = std::chrono::system_clock::now();

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		GLUtil::bindBuffers(cylinderBuffers);
		//Draw axes
		//Y axis	
		glUseProgram(simple3d.id);
		modelViewMat = glm::rotate(glm::mat4(1.0f), camRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *	   
					   glm::rotate(glm::mat4(1.0f), camRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
					   glm::translate(glm::mat4(1.0f), -camPos) *
					   glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -500.0f, 0.0f)) * 
					   glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 1000.0f, 0.02f));
		glUniformMatrix4fv(simple3d.uniforms["uPerspMat"], 1, false, glm::value_ptr(persp));	
		glUniformMatrix4fv(simple3d.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
		glUniform4f(simple3d.uniforms["uCol"], 1.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, cylinderObj.verts.size());

		//Z axis
		modelViewMat = glm::rotate(glm::mat4(1.0f), camRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *	   
					   glm::rotate(glm::mat4(1.0f), camRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
					   glm::translate(glm::mat4(1.0f), -camPos) *
					   glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -500.0f)) *
					   glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
					   glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 1000.0f, 0.02f));
		glUniformMatrix4fv(simple3d.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
		glUniform4f(simple3d.uniforms["uCol"], 0.0f, 1.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, cylinderObj.verts.size());

		//X axis
		modelViewMat = glm::rotate(glm::mat4(1.0f), camRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *	   
					   glm::rotate(glm::mat4(1.0f), camRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
					   glm::translate(glm::mat4(1.0f), -camPos) *
					   glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 0.0f, 0.0f)) *
					   glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
					   glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *	 
					   glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 1000.0f, 0.02f));
		glUniformMatrix4fv(simple3d.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
		glUniform4f(simple3d.uniforms["uCol"], 0.0f, 0.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, cylinderObj.verts.size());
		
		switch(viewMode)
		{
		case NORMAL:
			//Draw the surface of rotation	
			glUseProgram(program.id);
			modelViewMat = glm::rotate(glm::mat4(1.0f), camRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *	   
						   glm::rotate(glm::mat4(1.0f), camRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						   glm::translate(glm::mat4(1.0f), -camPos);
			glUniform1f(program.uniforms["uMaxAngle"], maxAngle);	
			glUniformMatrix4fv(program.uniforms["uPerspMat"], 1, false, glm::value_ptr(persp));	
			glUniformMatrix4fv(program.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
			glDrawArraysInstanced(GL_TRIANGLES, 0, cylinderObj.verts.size(), 2560);
			break;	
		case FLAT:
			//Draw original graph
			glUseProgram(program.id);
			GLUtil::bindBuffers(lineBuffers);
			modelViewMat = glm::rotate(glm::mat4(1.0f), camRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *	   
						   glm::rotate(glm::mat4(1.0f), camRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						   glm::translate(glm::mat4(1.0f), -camPos);			
			glUniform1f(program.uniforms["uMaxAngle"], maxAngle);		
			glUniformMatrix4fv(program.uniforms["uPerspMat"], 1, false, glm::value_ptr(persp));	
			glUniformMatrix4fv(program.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
			glDrawArraysInstanced(GL_LINES, 0, lineObj.verts.size(), 2560);
			break;	
		case ANIMATION:
			//Original graph rotates around
			maxAngle += ANIMATION_SPEED * secondsPerFrame;
			if(maxAngle > 3 * PI)
				maxAngle = 0.0f;
			glUseProgram(program.id);
			modelViewMat = glm::rotate(glm::mat4(1.0f), camRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *	   
						   glm::rotate(glm::mat4(1.0f), camRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						   glm::translate(glm::mat4(1.0f), -camPos);	
			glUniform1f(program.uniforms["uMaxAngle"], maxAngle);	
			glUniformMatrix4fv(program.uniforms["uPerspMat"], 1, false, glm::value_ptr(persp));	
			glUniformMatrix4fv(program.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
			glDrawArraysInstanced(GL_TRIANGLES, 0, cylinderObj.verts.size(), 2560);	
			break;
		}	

		updateCamera(secondsPerFrame);

		double cursorX, cursorY;
		glfwGetCursorPos(win, &cursorX, &cursorY);
		cursorPositionCallback(win, cursorX, cursorY);

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
