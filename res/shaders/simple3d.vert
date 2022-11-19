#version 460

layout(location = 0) in vec4 pos;

uniform mat4 uPerspMat;
uniform mat4 uMVMat = mat4(1.0);

//Main function
void main()
{
	gl_Position = uPerspMat * uMVMat * pos;	
}
