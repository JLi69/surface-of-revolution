#ifndef GL_UTIL
#include <vector>
#include <string>
#include <map>

#define BUFFER_COUNT 1
#define MAX_LOG_LEN 1024
#define PI 3.1415926535f

//Basic OpenGL functions to make my life easier
namespace GLUtil
{
	//Buffer object
	struct Buffers
	{
		unsigned int arr; //Vertex Array
		unsigned int buff[BUFFER_COUNT]; //Vertex Buffer
	};

	struct ShaderProgram
	{
		unsigned int id;
		std::map<std::string, int> uniforms;
	};

	struct BufferValues
	{
		std::vector<float> verts;
	};

	//Handle shaders
	std::string readShaderFile(const std::string path);
	int compileShader(unsigned int shader, unsigned int shaderType);	
	unsigned int createShaderProgram(
			const std::string vertShaderPath,
			std::string fragShaderPath
			);	
	
	//Create vertex buffers
	Buffers genVertBuffers(BufferValues buffVals);
	//Bind vertex buffer
	void bindBuffers(Buffers buffers);

	//Output any OpenGL errors
	void outputGLErrors();

	//Generate cylinder
	BufferValues generateCylinder(int prec);

#ifdef TEST
	//Test rectangle
	unsigned int createTestRectangle();
#endif
}
#endif
#define GL_UTIL
