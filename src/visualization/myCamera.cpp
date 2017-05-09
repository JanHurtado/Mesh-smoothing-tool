#include "myCamera.h"

myCamera::myCamera() :
viewDirection(0.0f, 0.0f, -1.0f),
position(0.f,0.f, 1.0f),
up(0.0f, 1.0f, 0.0f)
{
	updateStrafeDirection();
	movementSpeed = 1.0f;
}

void myCamera::updateStrafeDirection()
{
	strafeDirection = glm::cross(viewDirection, up);
	strafeDirection = glm::normalize(strafeDirection);
}

glm::mat4 myCamera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, up);
}

void myCamera::moveForward()
{
	position += movementSpeed * viewDirection;
}

void myCamera::moveBackward()
{
	position += -movementSpeed * viewDirection;
}

void myCamera::strafeLeft()
{
	position += -movementSpeed * strafeDirection;
}

void myCamera::strafeRight()
{
	position += movementSpeed * strafeDirection;
}

void myCamera::moveUp()
{
	position += movementSpeed * up;
}

void myCamera::moveDown()
{
	position += -movementSpeed * up;
}
