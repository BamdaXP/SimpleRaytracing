#include "Ray.hpp"

Ray::Ray()
	:origin({ 0.0f,0.0f,0.0f }), direction({1.0f,0.0f,0.0f})
{

}
Ray::Ray(glm::vec3 o, glm::vec3 d)
	:origin(o),direction(glm::normalize(d))
{

}
