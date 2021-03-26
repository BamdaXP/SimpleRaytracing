#include "Geometry.hpp"
Geometry::Geometry(const glm::vec3& p)
	:position(p)
{
}



Plane::Plane(const glm::vec3& p, const glm::vec3& n)
	:Geometry(p),normal(glm::normalize(n))

{
}

float Plane::Intersect(const Ray& r) const
{	
	float height = glm::dot(r.origin - position, normal);
	//If ray origin is behind the plane:miss
	if (height <= 0)
	{
		return 0;
	}
	else
	{
		float cos = glm::dot(r.direction, normal);
		//If ray is not aiming at the plane to the plane:miss
		if ( cos >= 0)
		{
			return 0;
		}
		else//hit
		{
			return  height / (-cos);
		}
	}
	

}

glm::vec3 Plane::GetNormal(const glm::vec3& hitPoint) const
{
	return normal;
}


Sphere::Sphere(const glm::vec3& p,float r)
	:Geometry(p),radius(r)
{
}

float Sphere::Intersect(const Ray& r) const
{
	auto op = position - r.origin;
	auto l = glm::dot(op, op);

	//solve equation
	//float a = 1;
	float b = -2 * glm::dot(op, r.direction)/*glm::sqrt(l)*/;
	float c = l - radius * radius;

	float beta = b * b - 4 * c;
	if (beta <= 0) return 0;
	
	float result = (-b - glm::sqrt(beta)) / 2;
	if (result < 0) return 0;
	return result;
}

glm::vec3 Sphere::GetNormal(const glm::vec3& hitPoint) const
{
	return glm::normalize(hitPoint - position);
}


