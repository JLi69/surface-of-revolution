#include "gl_util.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GLUtil::BufferValues GLUtil::generateCylinder(int prec)
{
	GLUtil::BufferValues cylinder;

	//Generate vertices
	glm::vec3 topPoint(1.0f, 1.0f, 0.0f), botPoint(1.0f, 0.0f, 0.0f);
	float rotationRad = 2.0f * PI / (float)prec;

	glm::vec3 centerTop(0.0f, 1.0f, 0.0f), centerBot(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < prec; i++)
	{
		//Rotate the points about the y axis
		glm::vec3 topPoint2 = glm::vec3(glm::vec4(topPoint, 1.0f) * glm::rotate(glm::mat4(1.0f), rotationRad, glm::vec3(0.0f, 1.0f, 0.0f))),
				  botPoint2 = glm::vec3(glm::vec4(botPoint, 1.0f) * glm::rotate(glm::mat4(1.0f), rotationRad, glm::vec3(0.0f, 1.0f, 0.0f)));

		glm::vec3 tris[] = 
		{
			//Triangle 1
			topPoint,
			botPoint,
			botPoint2,
			//Triangle 2
			topPoint, 
			botPoint2,
			topPoint2,
			/*		
			topPoint2,
			centerTop,	
			topPoint,
	
			botPoint2,
			centerTop,
			botPoint,
			*/	
		};

		for(int j = 0; j < 6; j++)
		{
			cylinder.verts.push_back(tris[j].x);
			cylinder.verts.push_back(tris[j].y);
			cylinder.verts.push_back(tris[j].z);
		}

		//std::cout << topPoint.x << ", " << topPoint.z << " | " << topPoint2.x << ", " << topPoint2.z << '\n';

		topPoint = topPoint2;
		botPoint = botPoint2;	
	}

	return cylinder;
}
