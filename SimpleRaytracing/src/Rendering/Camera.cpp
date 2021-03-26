#include "Camera.hpp"

Camera::Camera(glm::vec3 p, glm::vec3 d) 
	:Position(p), Direction(glm::normalize(d)) 
{
}