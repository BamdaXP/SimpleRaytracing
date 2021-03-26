#include "Object.hpp"

Object::Object(std::shared_ptr<Geometry> geo, std::shared_ptr<Material> mat)
	:geometry(geo),material(mat)
{
}
