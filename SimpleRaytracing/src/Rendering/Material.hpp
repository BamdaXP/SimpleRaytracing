#pragma once

#include "Geometry/Ray.hpp"

#include <glm/glm.hpp>
#include <vector>
class Material 
{
public:
	struct WeightedDirection
	{
	public:
		float Weight;
		glm::vec3 Direction;
	public:
		WeightedDirection(glm::vec3& d, float w=1.0f) :Weight(w), Direction(d) {}
		WeightedDirection(float x, float y, float z, float w=1.0f) : Direction{ x,y,z }, Weight(w) {}
		
	};
public:
	glm::vec3 color;
	glm::vec3 emission;
public:
	Material(glm::vec3 c, glm::vec3 e);
	//Return the color should be blent and modify the multiple weighted outDirs
	virtual glm::vec3 RayInteraction(const glm::vec3& inDir,const glm::vec3& normal,const glm::vec3& hitPoint,const uint16_t depth, std::vector<WeightedDirection>& outDirs) const = 0;

};

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(glm::vec3 c, glm::vec3 e);

	virtual glm::vec3 RayInteraction(const glm::vec3& inDir, const glm::vec3& normal, const glm::vec3& hitPoint, const uint16_t depth, std::vector<WeightedDirection>& outDirs) const override;
};

class ReflectalMaterial : public Material
{
public:
	ReflectalMaterial(glm::vec3 c, glm::vec3 e);
	virtual glm::vec3 RayInteraction(const glm::vec3& inDir, const glm::vec3& normal, const glm::vec3& hitPoint, const uint16_t depth, std::vector<WeightedDirection>& outDirs) const override;
};

class RefractalMaterial : public Material
{
public:
	float AirRefractionCoefficient;
	float MaterialRefractionCoefficient;
public:
	RefractalMaterial(glm::vec3 c, glm::vec3 e, float airN=1.0f,float matN = 1.6f);

	virtual glm::vec3 RayInteraction(const glm::vec3& inDir, const glm::vec3& normal, const glm::vec3& hitPoint, const uint16_t depth, std::vector<WeightedDirection>& outDirs) const override;
};