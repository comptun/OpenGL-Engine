#include "Camera.h"

Camera::Camera(const glm::vec3& position)
	: view(glm::translate(glm::mat4(1.0f), position)), pos(position), up(0.0f, 1.0f, 0.0f), front(0.0f, 0.0f, -1.0f)
{

}
Camera::~Camera()
{

}

void Camera::translate(const glm::vec3& t)
{
	view = glm::translate(view, t);
	pos += t;
}
void Camera::setPosition(const glm::vec3& p)
{
	view = glm::translate(glm::mat4(1.0f), -p);
	pos = -p;
}
glm::mat4 Camera::getViewMatrix() const { return view; }
glm::vec3 Camera::getPos() const { return pos; }
glm::vec3 Camera::getUp() const { return up; }
glm::vec3 Camera::getFront() const { return front; }
void Camera::setUp(const glm::vec3& up) { this->up = up; }
void Camera::setFront(const glm::vec3& front) { this->front = front; }
void Camera::setViewMatrix(const glm::mat4& mat) { this->view = mat; }
