#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_util.h"

#define DEFAULT_WIDTH 900
#define DEFAULT_HEIGHT 600

//Perspective matrix
static glm::mat4 persp = glm::perspective(75.0f, (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT, 0.1f, 1000.0f);
//Model view matrix
static glm::mat4 modelViewMat = glm::mat4(1.0f);

void handleWinResize(GLFWwindow *win, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	persp = glm::perspective(75.0f, (float)newWidth / (float)newHeight, 0.1f, 1000.0f);
}

int main()
{
	//Initailize window
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Surface of Revolution", NULL, NULL);
	glfwMakeContextCurrent(win);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetWindowSizeCallback(win, handleWinResize);

	//Create cylinder
	GLUtil::BufferValues cylinderObj = GLUtil::generateCylinder(48);
	GLUtil::Buffers cylinderBuffers = GLUtil::genVertBuffers(cylinderObj);
	GLUtil::bindBuffers(cylinderBuffers);

	//Create shader program
	GLUtil::ShaderProgram program;
	program.id = GLUtil::createShaderProgram("res/shaders/3d.vert", "res/shaders/simple.frag");
	program.uniforms["uPerspMat"] = glGetUniformLocation(program.id, "uPerspMat");
	program.uniforms["uMVMat"] = glGetUniformLocation(program.id, "uMVMat");	
	glUseProgram(program.id);

	modelViewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -20.0f));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while(!glfwWindowShouldClose(win))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(program.uniforms["uPerspMat"], 1, false, glm::value_ptr(persp));	
		glUniformMatrix4fv(program.uniforms["uMVMat"], 1, false, glm::value_ptr(modelViewMat));
		glDrawArrays(GL_TRIANGLES, 0, cylinderObj.verts.size());	

		GLUtil::outputGLErrors();
		glfwPollEvents();
		glfwSwapBuffers(win);
	}
	glfwTerminate();
}
