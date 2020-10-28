#pragma once

#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "const.h"

NS_CC_BEGIN

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(_pos, _pos + _front, _up);
	}

	glm::mat4 getProjectionMatrix() const
	{
		return glm::perspective(glm::radians(_fov), _ratio, _near, _far);
	}

	void move(Direction d, double deltaTime);
	void changeDir(float xoffset, float yoffset);
	void zoom(float yoffset);

	void setPos(float x,float y, float z) {
		_pos.x = x;
		_pos.y = y;
		_pos.z = z;
	}

	void lookAt(float x, float y, float z) {

	}

	void setPitch(float pitch) {
		_pitch = pitch;
		updateCameraVectors();
	}

	void setYaw(float yaw) {
		_yaw = yaw;
		updateCameraVectors();
	}

	glm::vec3 getPos() const {
		return _pos;
	}

private:
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		front.y = sin(glm::radians(_pitch));
		front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		_right = glm::normalize(glm::cross(_front, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		_up = glm::normalize(glm::cross(_right, _front));
	}

private:
	glm::vec3 _pos;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;
	float _yaw;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float _pitch;
	float _fov;
	float _ratio; // ³¤¿í±È
	float _near;
	float _far;
};

NS_CC_END