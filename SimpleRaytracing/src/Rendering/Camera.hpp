#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Direction;

	Camera(glm::vec3 p, glm::vec3 d);
};