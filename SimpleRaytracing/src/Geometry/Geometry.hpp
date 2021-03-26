#pragma once
#include <glm/glm.hpp>
#include "Ray.hpp"


class Geometry 
{
public:
	glm::vec3 position;
public:
	Geometry(const glm::vec3& p);
	/*Return the contact point distance from the ray origin
	Return 0 for miss*/
	virtual float Intersect(const Ray& r) const = 0;
	virtual glm::vec3 GetNormal(const glm::vec3& hitPoint) const = 0;
};


class Plane : public Geometry
{
public:
	glm::vec3 normal;
public:
	Plane(const glm::vec3& p, const glm::vec3& n);
	virtual float Intersect(const Ray& r) const override;
	virtual glm::vec3 GetNormal(const glm::vec3& hitPoint) const override;
};

class Sphere : public Geometry
{
public:
	float radius;
public:
	Sphere(const glm::vec3& p, float_t r);
	virtual float Intersect(const Ray& r) const override;
	virtual glm::vec3 GetNormal(const glm::vec3& hitPoint) const override;
};