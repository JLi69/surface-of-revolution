#include <fstream>
#include <sstream>
#include <iostream>
#include "gl_util.h"
#include <glad/glad.h>

std::string GLUtil::readShaderFile(const std::string path)
{
	std::ifstream file(path);

	//File not opened
	if(!file.is_open())
	{
		std::cout << "Error, cannot open file: " << path << '\n';
		file.close();	
		return "\0";
	}

	//Read the file
	std::stringstream srcSS;
	std::string line;
	while(std::getline(file, line)) srcSS << line << '\n';

	file.close();
	return srcSS.str();
}

int GLUtil::compileShader(unsigned int shader, unsigned int shaderType)
{
	int compiled;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	//Error handling
	if(!compiled)
	{
		char log[MAX_LOG_LEN + 1];
		int len;
		glGetShaderInfoLog(shader, MAX_LOG_LEN, &len, log);
		log[len] = '\0';

		switch(shaderType)
		{
		case GL_VERTEX_SHADER: std::cout << "Vertex shader failed to compile!\n"; break;
		case GL_FRAGMENT_SHADER: std::cout << "Fragment shader failed to compile!\n"; break;
		default: std::cout << "Shader failed to compile!\n"; break;
		}
		std::cout << log << '\n';
	}

	return compiled;
}

unsigned int GLUtil::createShaderProgram(const std::string vertShaderPath,
								 const std::string fragShaderPath)
{
	unsigned int program = glCreateProgram();
	unsigned int vert = glCreateShader(GL_VERTEX_SHADER),
				 frag = glCreateShader(GL_FRAGMENT_SHADER);

	//Source code for shaders
	std::string vertSrc = readShaderFile(vertShaderPath), 
				fragSrc = readShaderFile(fragShaderPath);
	//Send the source code to the shaders
	const char* ptr;	
	//Vertex shader	
	ptr = vertSrc.c_str();
	glShaderSource(vert, 1, &ptr, nullptr);	
	//Fragment shader	
	ptr = fragSrc.c_str();	
	glShaderSource(frag, 1, &ptr, nullptr);

	//Compile shaders
	compileShader(vert, GL_VERTEX_SHADER);	
	compileShader(frag, GL_VERTEX_SHADER);
	//Attach and link shaders
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glValidateProgram(program);
	//Clean up
	glDetachShader(program, vert);
	glDetachShader(program, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}

GLUtil::Buffers GLUtil::genVertBuffers(BufferValues buffVals)
{
	GLUtil::Buffers buffers;

	glGenVertexArrays(1, &buffers.arr);
	glBindVertexArray(buffers.arr);

	glGenBuffers(BUFFER_COUNT, buffers.buff);

	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers.buff[0]);
	glBufferData(GL_ARRAY_BUFFER, buffVals.verts.size() * sizeof(float), 
				 &buffVals.verts.at(0), GL_STATIC_DRAW);
	

	return buffers;
}

void GLUtil::bindBuffers(GLUtil::Buffers buffers)
{	
	glBindVertexArray(buffers.arr);

	glBindBuffer(GL_ARRAY_BUFFER, buffers.buff[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
}

void GLUtil::outputGLErrors()
{
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGL Error: " << err << '\n';
}

GLUtil::BufferValues GLUtil::generateLine()
{
	GLUtil::BufferValues line;

	//Generate vertices
	line.verts = {
		1.0f, 1.0f, 0.0f,	
		1.0f, 0.0f, 0.0f
	};

	return line;
}

#ifdef TEST
unsigned int GLUtil::createTestRectangle()
{
	float rect[] = 
	{
		1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,

		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};

	unsigned int rectangleBuff;
	glGenBuffers(1, &rectangleBuff);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	return rectangleBuff;
}
#endif
