#pragma once 
#include "Geometry/Geometry.hpp"
#include "Material.hpp"
#include <memory>
class Object 
{
public:
	std::shared_ptr<Geometry> geometry = nullptr;
	std::shared_ptr<Material> material = nullptr;
public:
	Object(std::shared_ptr<Geometry> geo, std::shared_ptr<Material> mat);
};