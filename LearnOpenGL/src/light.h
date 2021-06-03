#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);
const glm::vec3 white = glm::vec3(1.0, 1.0, 1.0);
const float A = 0.0032;
const float B = 0.09;
const float C = 1.0;

class Light
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(glm::vec3 setAmbient, glm::vec3 setDiffuse, glm::vec3 setSpecular)
	{
		ambient = setAmbient;
		diffuse = setDiffuse;
		specular = setSpecular;
	}
};

class DirectionLight : public Light {
public:
	glm::vec3 direction;
	DirectionLight(glm::vec3 setAmbient, glm::vec3 setDiffuse, glm::vec3 setSpecular,
				   glm::vec3 setDirection)
		: Light(setAmbient, setDiffuse, setSpecular)
	{
		direction = setDirection;
	}
};
class PointLight : public Light {
public:
	glm::vec3 position;
	float a;
	float b;
	float c;

	PointLight(glm::vec3 setAmbient  = origin, 
			   glm::vec3 setDiffuse  = origin, 
			   glm::vec3 setSpecular = white,
			   glm::vec3 setPosition = origin, 
			   float setA = A, 
			   float setB = B, 
			   float setC = C)
		: Light(setAmbient, setDiffuse, setSpecular)
	{
		position = setPosition;
		a = setA;
		b = setB;
		c = setC;
	}
};
class SpotLight : public Light {
public:
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutOff;
	float outerCutOff;

	float a;
	float b;
	float c;

	SpotLight(glm::vec3 setAmbient, glm::vec3 setDiffuse, glm::vec3 setSpecular,
			  glm::vec3 setPosition, glm::vec3 setDirection,
			  float setInnerCutOff, float setOuterCutOff,
			  float setA, float setB, float setC)
		: Light(setAmbient, setDiffuse, setSpecular)
	{
		position = setPosition;
		direction = setDirection;
		innerCutOff = glm::cos(glm::radians(setInnerCutOff));
		outerCutOff = glm::cos(glm::radians(setOuterCutOff));

		a = setA;
		b = setB;
		c = setC;
	}
};
#endif