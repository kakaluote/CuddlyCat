#include "Camera.h"


NS_CC_BEGIN


Camera::Camera():
	_pos(0.0f, 0.0f, 3.0f),
	_front(0.0f, 0.0f, -1.0f),
	_up(0.0f, 1.0f, 0.0f),
	_yaw(-90.0f),
	_pitch(0.0f),
	_fov(45.0f),
	_worldUp(0.0f, 1.0f, 0.0f),
	_ratio(1.33333f),
	_near(0.1f),
	_far(100.0f)
{
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::move(Direction d, double deltaTime)
{
	float velocity = 0.01f * deltaTime;
	if (d == FORWARD)
		_pos += _front * velocity;
	if (d == BACKWARD)
		_pos -= _front * velocity;
	if (d == LEFT)
		_pos -= _right * velocity;
	if (d == RIGHT)
		_pos += _right * velocity;
	if (d == UP)
		_pos += _up * velocity;
	if (d == DOWN)
		_pos -= _up * velocity;
}

void Camera::changeDir(float xoffset, float yoffset)
{
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::zoom(float yoffset)
{
	if (_fov >= 1.0f && _fov <= 45.0f)
		_fov -= yoffset;
	if (_fov <= 1.0f)
		_fov = 1.0f;
	if (_fov >= 45.0f)
		_fov = 45.0f;
}

NS_CC_END




