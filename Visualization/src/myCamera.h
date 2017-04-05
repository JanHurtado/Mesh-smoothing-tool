#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <glm\glm.hpp>
#include <iostream>

using namespace std;

class myCamera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 UP;
	glm::vec2 oldMousePosition;
	static const float MOVEMENT_SPEED;
	glm::vec3 strafeDirection;
public:
	myCamera();
	bool mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
	glm::vec3 getPosition() const { return position; }

	void setPosition(glm::vec3 _pos){ position = _pos; }

	glm::vec3 objectCentroid;
};

#endif // MYCAMERA_H