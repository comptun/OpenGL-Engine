#pragma once

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Camera
{
private:
	glm::mat4 view;
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 front;
	
public:
	Camera(const glm::vec3& position);
	~Camera();

	void translate(const glm::vec3& t);
	void setPosition(const glm::vec3& p);
	glm::mat4 getViewMatrix() const;
	glm::vec3 getPos() const;
	glm::vec3 getUp() const;
	glm::vec3 getFront() const;

	void setUp(const glm::vec3& up);
	void setFront(const glm::vec3& front);
	void setViewMatrix(const glm::mat4& mat);
};

