#include "camera.h"
Camera::Camera() :
	position(glm::vec3(0,0,0)),
	target(glm::vec3(0,0,0)),
	viewMat(glm::mat4(1.0f))
{
}

void Camera::lookAt(float targetX, float targetY, float targetZ)
{
	target = glm::vec3(targetX, targetY, targetZ);
	viewMat = glm::lookAt(
		position,
		target,
		glm::vec3(0, 1.0f, 0)
	);
}

void Camera::moveTo(float posX, float posY, float posZ)
{
	position = glm::vec3(posX, posY, posZ);
	viewMat = glm::lookAt(
		position,
		target,
		glm::vec3(0, 1.0f, 0)
	);
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMat;
}
