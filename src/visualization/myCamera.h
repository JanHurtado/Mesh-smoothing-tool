#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>

using namespace std;

class myCamera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 up;
	glm::vec3 strafeDirection;

	float movementSpeed;

public:
	myCamera();
	void updateStrafeDirection();
	glm::mat4 getWorldToViewMatrix() const;
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getViewDirection() const { return viewDirection; }
	glm::vec3 getUP() const { return up; }
	glm::vec3 getStrafeDirection() const { return strafeDirection; }
	float getMovementSpeed() const { return movementSpeed; };

	void setPosition(glm::vec3 & _pos){ position = _pos; }
	void setViewDirection(glm::vec3 & _viewDirection){ viewDirection = _viewDirection; }
	void setUp(glm::vec3 & _up){ up = _up; }
	void setStrafeDirection(glm::vec3 & _strafeDirection){ strafeDirection = _strafeDirection; }
	void setMovementSpeed(float _movementSpeed){ movementSpeed = _movementSpeed; }
};

#endif // MYCAMERA_H