#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW			= 0.0f;
const float PITCH		= 0.0f;
const float SPEED		= 5.0f;
const float SENSITIVITY = 0.07f;
const float ZOOM		= 100.0f;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 setPosition = glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3 setWorldUp = glm::vec3(0.0f, 0.0f, 1.0f),
		float setYaw = YAW,
		float setPitch = PITCH)
		:
		front(glm::vec3(-1.0f, 0.0f, 0.0f)),
		movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		position = setPosition;
		worldUp = setWorldUp;
		yaw = setYaw;
		pitch = setPitch;
	}
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}
	void processKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
	}
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch -= yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (pitch > 89.99f)
				pitch = 89.99f;
			if (pitch < -89.99f)
				pitch = -89.99f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}
	void processMouseScroll(float yoffset)
	{
		zoom += yoffset/10.0f;
	}
private:
	void updateCameraVectors()
	{
		glm::vec3 direction;
		direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.z = sin(glm::radians(pitch));
		front = glm::normalize(direction);

		right = glm::normalize(glm::cross(worldUp, front));
		up	  = glm::normalize(glm::cross(right, front));
	}
};

#endif