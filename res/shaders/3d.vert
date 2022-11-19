#version 460

#define PI 3.1415926535

layout(location = 0) in vec4 pos;

uniform mat4 uPerspMat;
uniform mat4 uMVMat = mat4(1.0);

float uPrec = 0.001;

out vec4 rainbow;

out float varyingAngle;

//x = f(y)
float fx(float y)
{	
	//Some example functions	
	
	//Cylinder	
	//return 1;
	
	//Sphere
	//return sqrt(4 - y * y);	
	
	//return y * y;

	//Parabola
	return sqrt(y);
	
	//Cone	
	//return y;
	
	//Trig functions generate fun stuff	
	//return sin(y);
	//return cos(sin(y * y)) * y;
	//return 1.0 / sin(y); 
	//return asin(y);
	//return y * y * sin(y * 5.0);

	//return 1.0 / y;

	//Peicewise function for fun
	/*if(y < 1.0)
		return sqrt(y);
	else if(y >= 1.0)
		return y;*/
}

//Main function
void main()
{
	vec4 transformed = vec4(pos.x,
							pos.y * 2.0 * uPrec, pos.z, 1.0) + vec4(0.0, 2.0 * (gl_InstanceID) * uPrec - 2560.0 * uPrec,
							0.0, 0.0);

	mat4 scale;
	scale[0] = vec4(fx(transformed.y), 0.0, 0.0, 0.0);
	scale[1] = vec4(0.0, -1.0, 0.0, 0.0);
	scale[2] = vec4(0.0, 0.0, fx(transformed.y), 0.0);
	scale[3] = vec4(0.0, 0.0, 0.0, 1.0);

	transformed = scale * transformed;
	
	gl_Position = uPerspMat * uMVMat * transformed;
	
	//Rainbow color	
	rainbow = vec4(transformed.xyz * 0.5 + vec3(0.5, 0.5, 0.5), 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);

	//Calculate angle
	varyingAngle = atan(pos.z / pos.x);
	if(pos.z < 0.0 && pos.x > 0.0)
		varyingAngle += 2 * PI;
	else if(pos.z > 0.0 && pos.x < 0.0)
		varyingAngle += PI;
	else if(pos.z < 0.0 && pos.x < 0.0)
		varyingAngle += PI;
}
