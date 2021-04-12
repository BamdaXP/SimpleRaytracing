#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 dir, float ratio, float fov)
	:Position(pos), Direction(glm::normalize(dir)),HWRatio(ratio),FovInRad(fov)
{
}