// The entry
#include "Rendering/Renderer.hpp"
#include "ThreadPool.hpp"
#include <vector>

using namespace glm;
int main()
{

	ThreadPool::Start();

	RendererDescription desc;
	desc.Height = 768;
	desc.Width = 1024;
	desc.SamplesPerPixel = 64;
	desc.SubPixels = 8;
	desc.MaxDepth = 4;
	desc.SavePathFileName = "./result.ppm";

	auto renderer = Renderer(desc);
	
	std::vector<Object> scene;
	scene.emplace_back(//Base plane
		std::make_shared<Plane>(vec3{ 0,-60,0 }, vec3{ 0,1,0 }),
		std::make_shared<DiffuseMaterial>(vec3{ 0.5,1,0 }, vec3{ 0,0,0 }));
	scene.emplace_back(//Base plane
		std::make_shared<Plane>(vec3{ 0,0,-200 }, vec3{ 0,0,1 }),
		std::make_shared<DiffuseMaterial>(vec3{ 1,0.5,1 }, vec3{ 0,0,0 }));
	scene.emplace_back(//Diffuse ball
		std::make_shared<Sphere>(vec3{ -60,-45,-150 },50),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 1,0.5,0.5 },vec3{0,0,0}));//color emission
	scene.emplace_back(//Diffuse ball
		std::make_shared<Sphere>(vec3{ 0,-40,-50 }, 40),//pos radius
		std::make_shared<ReflectalMaterial>(vec3{ 1,1,1 }, vec3{ 0,0,0 }));//color emission
	scene.emplace_back(//Diffuse ball
		std::make_shared<Sphere>(vec3{ 30,-35,20 }, 30),//pos radius
		std::make_shared<RefractalMaterial>(vec3{ 1,1,1 }, vec3{ 0,0,0 }));//color emission
	/*
	scene.emplace_back(//Reflectal ball
		std::make_shared<Sphere>(vec3{ -15,-30,-15 }, 20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 1,0.5,0.5 }, vec3{ 0,0,0 }));//color emission
	scene.emplace_back(//Refractal ball
		std::make_shared<Sphere>(vec3{ 15,-30,15 }, 20),//pos radius
		std::make_shared<RefractalMaterial>(vec3{ 1,0.5,0.5 }, vec3{ 0,0,0 }));//color emission
	*/
	scene.emplace_back(//Light blob
		std::make_shared<Sphere>(vec3{ 0,50,0 },20),//pos radius
		std::make_shared<DiffuseMaterial>(vec3{ 0,0,0 },vec3{ 15,15,15 }));//color emission
	
	auto camera = Camera({ 0,0,300 }, { 0,0,-1 },0.5f,PI/4);


	renderer.Render(camera, scene);

	return 0;
}
