#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Direction;
	float HWRatio;
	float FovInRad;
	Camera(glm::vec3 pos, glm::vec3 dir, float ratio, float fov);
};