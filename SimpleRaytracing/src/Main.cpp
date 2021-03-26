// The entry
#include "Rendering/Renderer.hpp"
#include <vector>

using namespace glm;
int main()
{
	RendererDescription desc;
	desc.Height = 768;
	desc.Width = 1024;
	desc.SamplesPerPixel = 64;
	desc.SubPixels = 4;
	desc.MaxDepth = 3;
	desc.SavePathFileName = "./result.ppm";

	auto renderer = Renderer(desc);
	
	std::vector<Object> scene;
	scene.emplace_back(//Base plane
		std::make_shared<Plane>(vec3{ 0,-60,0 }, vec3{ 0,1,0 }),
		std::make_shared<DiffuseMaterial>(vec3{ 0.5,0.5,1 }, vec3{ 0,0,0 }));
	scene.emplace_back(//Diffuse ball
		std::make_shared<Sphere>(vec3{ 0,40,0 },20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 1,0.5,0.5 },vec3{0,0,0}));//color emission
	scene.emplace_back(//Diffuse ball
		std::make_shared<Sphere>(vec3{ 40,0,0 }, 20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 0.8,0.8,0.1 }, vec3{ 0,0,0 }));//color emission
	scene.emplace_back(//Diffuse ball
		std::make_shared<Sphere>(vec3{ 0,-40,0 }, 20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 0.3,1,0.5 }, vec3{ 0,0,0 }));//color emission
	/*
	scene.emplace_back(//Reflectal ball
		std::make_shared<Sphere>(vec3{ -15,-30,-15 }, 20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 1,0.5,0.5 }, vec3{ 0,0,0 }));//color emission
	scene.emplace_back(//Refractal ball
		std::make_shared<Sphere>(vec3{ 15,-30,15 }, 20),//pos radius
		std::make_shared<RefractalMaterial>(vec3{ 1,0.5,0.5 }, vec3{ 0,0,0 }));//color emission
	*/
	scene.emplace_back(//Light blob
		std::make_shared<Sphere>(vec3{ 0,0,0 },20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 0,0,0 },vec3{ 3,3,3 }));//color emission
	
	auto camera = Camera({ 0,0,300 }, { 0,0,-1 });


	renderer.Render(camera, scene);
	return 0;
}
