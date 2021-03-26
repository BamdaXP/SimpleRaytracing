#pragma once

#include <glm/glm.hpp>
struct Ray 
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
public:
	Ray();
	Ray(glm::vec3 o, glm::vec3 d);
};