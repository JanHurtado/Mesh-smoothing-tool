#include "myCamera.h"

#include <glm\gtx\transform.hpp>
#include <algorithm>

const float myPI = 3.1415f;

myCamera::myCamera() :
viewDirection(0.0f, 0.0f, -1.0f),
position(0.f,0.f, 80.f),
UP(0.0f, 1.0f, 0.0f)
{
	viewDirection = glm::normalize(objectCentroid - position);
	UP = glm::normalize(UP);
	strafeDirection = glm::cross(viewDirection, UP);
	strafeDirection = glm::normalize(strafeDirection);
	MOVEMENT_SPEED = 1.0f;
}

void myCamera::reinitialize()
{
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	position = glm::vec3(0.f, 0.f, 80.f);
	UP = glm::vec3(0.0f, 1.0f, 0.0f);
	viewDirection = glm::normalize(objectCentroid - position);
	UP = glm::normalize(UP);
	strafeDirection = glm::cross(viewDirection, UP);
	strafeDirection = glm::normalize(strafeDirection);
	MOVEMENT_SPEED = 1.0f;
}

bool myCamera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 10.0f || glm::length(mouseDelta) < 1.0f)
	{
		oldMousePosition = newMousePosition;
		viewDirection = glm::normalize(objectCentroid - position);
		return false;
	}
	const float ROTATIONAL_SPEED = 0.1f;
	//viewDirection = glm::normalize(objectCentroid - position);
	strafeDirection = glm::cross(viewDirection, UP);
	strafeDirection = glm::normalize(strafeDirection);
	//strafeDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	//glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP);
	float valx = -mouseDelta.x * ROTATIONAL_SPEED;
	float valy = -mouseDelta.y * ROTATIONAL_SPEED;
	glm::mat4 rotator = glm::rotate(valx, UP) * glm::rotate(valy, strafeDirection);
	//glm::mat4 rotator = glm::rotate(valx, UP);
	//glm::mat4 rotator = glm::rotate(valy, strafeDirection);

	viewDirection = glm::mat3(rotator) * viewDirection;
	UP = glm::mat3(rotator) * UP;
	//position = glm::mat3(rotator) * position;
	cout << position.x << " " << position.y << " " << position.z << " " << endl;
	//viewDirection = glm::normalize(objectCentroid - position);
	oldMousePosition = newMousePosition;
	return true;
}

glm::mat4 myCamera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void myCamera::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;
}

void myCamera::moveBackward()
{
	position += -MOVEMENT_SPEED * viewDirection;
}

void myCamera::strafeLeft()
{
	position += -MOVEMENT_SPEED * strafeDirection;
}

void myCamera::strafeRight()
{
	position += MOVEMENT_SPEED * strafeDirection;
}

void myCamera::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}

void myCamera::moveDown()
{
	position += -MOVEMENT_SPEED * UP;
}