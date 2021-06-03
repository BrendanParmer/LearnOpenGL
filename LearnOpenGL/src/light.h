#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	virtual void something() = 0; //to make light abstract
};

class Direction : public Light {
public:
	glm::vec3 direction;
	Direction(glm::vec3 setAmbient, glm::vec3 setDiffuse, glm::vec3 setSpecular,
		glm::vec3 setDirection)
		: Light(setAmbient, setDiffuse, setSpecular)
	{
		direction = setDirection;
	}
};
class Point : public Light {
public:
	glm::vec3 position;
	float a;
	float b;
	float c;

	Point(glm::vec3 setAmbient, glm::vec3 setDiffuse, glm::vec3 setSpecular,
		glm::vec3 setPosition, float setA, float setB, float setC)
	: Light(setAmbient, setDiffuse, setSpecular)
	{
		position = setPosition;
		a = setA;
		b = setB;
		c = setC;
	}
};
class Spot : public Light {
public:
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutOff;
	float outerCutOff;

	float a;
	float b;
	float c;

	Spot(glm::vec3 setAmbient, glm::vec3 setDiffuse, glm::vec3 setSpecular,
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