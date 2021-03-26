#include "Material.hpp"
#include "Random.hpp"

#include <ctime>
#include <vector>
#define PI 3.1415926535897f

Material::Material(glm::vec3 c, glm::vec3 e)
	:color(c), emission(e)
{
}

DiffuseMaterial::DiffuseMaterial(glm::vec3 c, glm::vec3 e)
	:Material(c,e)
{
}


glm::vec3 DiffuseMaterial::RayInteraction(const glm::vec3& inDir, const glm::vec3& normal, const glm::vec3& hitPoint, uint16_t depth, std::vector<WeightedDirection>& outDirs) const
{
	glm::vec3 normal_l = glm::dot(normal, inDir) < 0 ? normal : -normal;

	float r1 = 2 * PI * Random::FRandom();
	float r2 = Random::FRandom();
	float r2sqrt = sqrt(r2);

	glm::vec3 u = glm::normalize(glm::cross((fabs(normal_l.x) > .1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), normal_l));
	glm::vec3 v = glm::cross(normal_l,u);

	outDirs.emplace_back(glm::normalize((u * cos(r1) * r2sqrt + v * sin(r1) * r2sqrt + normal_l * sqrt(1 - r2))));
	return color;
}





ReflectalMaterial::ReflectalMaterial(glm::vec3 c, glm::vec3 e)
	:Material(c, e)
{
}

glm::vec3 ReflectalMaterial::RayInteraction(const glm::vec3& inDir, const glm::vec3& normal, const glm::vec3& hitPoint, uint16_t depth, std::vector<WeightedDirection>& outDirs) const
{
	outDirs.emplace_back(inDir - glm::dot(normal,inDir)*2*normal);
	return color;
}



RefractalMaterial::RefractalMaterial(glm::vec3 c, glm::vec3 e, float airN, float matN)
	:Material(c,e),AirRefractionCoefficient(airN),MaterialRefractionCoefficient(matN)
{

}

glm::vec3 RefractalMaterial::RayInteraction(const glm::vec3& inDir, const glm::vec3& normal, const glm::vec3& hitPoint, uint16_t depth, std::vector<WeightedDirection>& outDirs) const
{
	glm::vec3 normal_l = glm::dot(normal, inDir) < 0 ? normal : -normal;
	glm::vec3 reflectDir = inDir - glm::dot(normal, inDir) * 2 * normal;
	
	bool into = glm::dot(normal,normal_l) > 0;
	
	//Relative refraction coefficient
	float nc = 1, nt = 1.5, nnt = into ? AirRefractionCoefficient / MaterialRefractionCoefficient : MaterialRefractionCoefficient / AirRefractionCoefficient;
	float ddn = glm::dot(inDir,normal_l);
	float cos2t;
	// total internal reflection
	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) 
	{
		outDirs.emplace_back(reflectDir);
		return color;
	}
	//Calculate the transmit direction
	glm::vec3 transmitDir = glm::normalize(inDir * nnt - normal * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t))));
	float a = nt - nc;
	float b = nt + nc;
	float R0 = a * a / (b * b);
	float c = 1 - (into ? -ddn : glm::dot(normal,transmitDir));

	//Amp of the vertical polarized ray
	float Re = R0 + (1 - R0) * c * c * c * c * c;
	float Tr = 1 - Re;
	//ray polarization porpotion
	float P = .25f + .5f * Re;
	//Amp of the horizontal polarized ray
	float RP = Re / P;
	float TP = Tr / (1 - P);
	
	//If depth is high
	if (depth > 2) {
		//Randomly choose the ray polarization base on the ray polarization porpotion
		if (Random::FRandom() < P) {
			outDirs.emplace_back(reflectDir);
			return RP * color;
		}
		else {
			outDirs.emplace_back(transmitDir);
			return TP * color;
		}
	}
	else {//If depth is low
		//Precisely calculate polarization
		outDirs.emplace_back(reflectDir, Re);
		outDirs.emplace_back(transmitDir, Tr);
		return color;
	}
}
