#version 460

in vec4 rainbow;
out vec4 color;
in float varyingAngle;
uniform float uMaxAngle = 999.0f;

void main()
{
	color = rainbow;	

	//For spinning animation
	if(varyingAngle > uMaxAngle)
		color = vec4(0.0, 0.0, 0.0, 0.0);
}
