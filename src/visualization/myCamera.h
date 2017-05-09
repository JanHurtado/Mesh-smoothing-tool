#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;

/** @addtogroup visualization
  * @brief Camera.
  *
  * @{
  */

/**
 * @brief The myCamera class - camera representation in a typical rendering pipeline (i.e. OpenGL)
 */
class myCamera
{
    /**
     * @brief position - camera position in world space (eye)
     */
    glm::vec3 position;
    /**
     * @brief viewDirection - camera view direction (should be unit vector)
     */
	glm::vec3 viewDirection;
    /**
     * @brief up - camera up direction (should be unit vector)
     */
	glm::vec3 up;
    /**
     * @brief strafeDirection - camera strafe direction (should be unit vector)
     */
	glm::vec3 strafeDirection;

    /**
     * @brief movementSpeed - movement speed for camera translation (forward, backward, left and right)
     */
	float movementSpeed;

public:
    /**
     * @brief myCamera - default constructor
     */
	myCamera();
    /**
     * @brief updateStrafeDirection - updates strafe direction vector given view direction and up vector
     */
	void updateStrafeDirection();
    /**
     * @brief getWorldToViewMatrix: generates world to view matrix
     * @return 4x4 world to view matrix
     */
	glm::mat4 getWorldToViewMatrix() const;
    /**
     * @brief moveForward - moves camera position following view direction (regarding movement speed)
     */
	void moveForward();
    /**
     * @brief moveBackward - moves camera position following the opposite direction of view direction (regarding movement speed)
     */
	void moveBackward();
    /**
     * @brief strafeLeft - moves camera position following the opposite direction of strafe direction (regarding movement speed)
     */
	void strafeLeft();
    /**
     * @brief strafeRight - moves camera position following the strafe direction (regarding movement speed)
     */
	void strafeRight();
    /**
     * @brief moveUp - moves camera position following up direction (regarding movement speed)
     */
	void moveUp();
    /**
     * @brief moveDown - moves camera position following the opposite direction of up direction (regarding movement speed)
     */
	void moveDown();

    /**
     * @brief getPosition - get camera position
     * @return camera position vector
     */
	glm::vec3 getPosition() const { return position; }
    /**
     * @brief getViewDirection - get view direction
     * @return camera view direction vector
     */
	glm::vec3 getViewDirection() const { return viewDirection; }
    /**
     * @brief getUP - get up direction
     * @return camera up direction vector
     */
	glm::vec3 getUP() const { return up; }
    /**
     * @brief getStrafeDirection - get strafe direction
     * @return camera strafe direction vector
     */
	glm::vec3 getStrafeDirection() const { return strafeDirection; }
    /**
     * @brief getMovementSpeed - get camera movement speed
     * @return camera movement speed value
     */
    float getMovementSpeed() const { return movementSpeed; }

    /**
     * @brief setPosition - set camera position
     * @param _pos new camera position
     */
	void setPosition(glm::vec3 & _pos){ position = _pos; }
    /**
     * @brief setViewDirection - set camera view direction
     * @param _viewDirection new camera view direction
     */
    void setViewDirection(glm::vec3 & _viewDirection){ viewDirection = _viewDirection; }
    /**
     * @brief setUp - set camera up direction
     * @param _up new camera up direction
     */
	void setUp(glm::vec3 & _up){ up = _up; }
    /**
     * @brief setStrafeDirection - set strafe direction
     * @param _strafeDirection new camera strafe direction
     */
	void setStrafeDirection(glm::vec3 & _strafeDirection){ strafeDirection = _strafeDirection; }
    /**
     * @brief setMovementSpeed - set camera movement speed
     * @param _movementSpeed new camera movement speed
     */
	void setMovementSpeed(float _movementSpeed){ movementSpeed = _movementSpeed; }
};

/** @} */

#endif // MYCAMERA_H
