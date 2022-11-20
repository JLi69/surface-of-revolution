#version 460

#define PI 3.1415926535

in vec4 rainbow;
out vec4 color;
in vec4 varyingPos;
uniform float uMaxAngle = 999.0f;

void main()
{
	color = rainbow;	
	
	//Calculate angle
	float angle = atan(varyingPos.z / varyingPos.x);
	if(varyingPos.z < 0.0 && varyingPos.x > 0.0)
		angle += 2 * PI;
	else if(varyingPos.z > 0.0 && varyingPos.x < 0.0)
		angle += PI;
	else if(varyingPos.z < 0.0 && varyingPos.x < 0.0)
		angle += PI;

	//For spinning animation
	if(angle > uMaxAngle)
		color = vec4(0.0, 0.0, 0.0, 0.0);
}
