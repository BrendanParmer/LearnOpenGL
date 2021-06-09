#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);
const glm::vec3 white = glm::vec3(1.0, 1.0, 1.0);
const float A = 0.032;
const float B = 0.09;
const float C = 1.0;

class Light
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
};

class DirectionLight : public Light {
public:
	glm::vec3 direction;
	DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				   glm::vec3 direction)
		: Light(ambient, diffuse, specular)
	{
		this->direction = direction;
	}
};
class PointLight : public Light {
public:
	glm::vec3 position;
	float a;
	float b;
	float c;

	PointLight(glm::vec3 ambient  = origin, 
			   glm::vec3 diffuse  = origin, 
			   glm::vec3 specular = white,
			   glm::vec3 position = origin, 
			   float a = A, 
			   float b = B, 
			   float c = C)
		: Light(ambient, diffuse, specular)
	{
		this->position = position;
		this->a = a;
		this->b = b;
		this->c = c;
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

	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
			  glm::vec3 position, glm::vec3 direction,
			  float innerCutOff, float outerCutOff,
			  float a, float b, float c)
		: Light(ambient, diffuse, specular)
	{
		this->position = position;
		this->direction = direction;
		this->innerCutOff = glm::cos(glm::radians(innerCutOff));
		this->outerCutOff = glm::cos(glm::radians(outerCutOff));

		this->a = a;
		this->b = b;
		this->c = c;
	}
};
#endif