#include "Renderer.hpp"
#include "ThreadPool.hpp"
#include "Random.hpp"

#include <iostream>
#include <fstream>

void Renderer::Render(const Camera& camera, const std::vector<Object>& scene)
{
	//Copy the scene into the renderer
	m_SceneBuffer.clear();
	m_SceneBuffer.reserve(scene.size());
	m_SceneBuffer.assign(scene.begin(), scene.end());


	uint16_t width = m_Description.Width;
	uint16_t height = m_Description.Height;
	//Camera info
	glm::vec3 camera_x = { width * camera.HWRatio / height ,0,0 };
	glm::vec3 camera_y = glm::normalize(glm::cross(camera_x, camera.Direction)) * camera.HWRatio;
	glm::vec3 focusPoint = camera.Position - camera.Direction * glm::length(camera_x / glm::tan(camera.FovInRad));

	std::cout << "Rendering" << std::endl
		<< scene.size() << "object(s) are(is) in the scene." << std::endl
		<< "Image size:" << width << "x" << height << std::endl;

	//bitmap result

	auto bitmap = std::vector<glm::vec3>(((uint32_t)width) * height);
	memset(bitmap.data(), 0, bitmap.size() * sizeof(glm::vec3));//clear bitmap



	ThreadPool::Start();


	//Logging
	uint32_t total = ((uint32_t)width) * height;
	float submitted = 0;//sync
	float finished = 0;//async
	std::mutex finishCountMutex;
	ThreadPool::SubmitTask([&]() {
		float lastSubmittedPercent = 0.0f;
		float lastFinishedPercent = 0.0f;
		//Block
		while (finished < total)
		{
			float currentSubmittedPercent = submitted / total;
			float currentFinishedPercent = finished / total;
			if ((int)(currentFinishedPercent * 100) > (int)(lastFinishedPercent * 100) || (int)(currentSubmittedPercent * 100) > (int)(lastSubmittedPercent * 100)) {
				std::cout << "\r";
				std::cout << "Submitted " << (int)(currentSubmittedPercent * 100) << "%\t" << "Finished " << (int)(currentFinishedPercent * 100) << "%";
			}
			lastSubmittedPercent = currentSubmittedPercent;
			lastFinishedPercent = currentFinishedPercent;
		}
		});


	//Render procedure
	for (uint16_t x = 0; x < width; x++)
	{
		for (uint16_t y = 0; y < height; y++)
		{
			//Procedure function
			auto proc = [&,x,y] {
				int i = (height - y - 1) * width + x;
				for (uint16_t subpixel = 0; subpixel < m_Description.SubPixels; subpixel++)
				{
					glm::vec3 radiance = { 0.0f,0.0f,0.0f };
					for (uint32_t sample = 0; sample < m_Description.SamplesPerPixel; sample++)
					{
						float dx = Random::FRandom(-1, 1);
						float dy = Random::FRandom(-1, 1);
						auto direction = camera.Direction + camera_x * ((dx + x) / width - 0.5f) + camera_y * ((dy + y) / height - 0.5f);
						radiance = radiance + Radiance(Ray(focusPoint, direction), 1) * (1.0f / m_Description.SamplesPerPixel);
					}
					bitmap[i] = bitmap[i] + glm::clamp(radiance, 0.0f, 1.0f) * (1.0f / m_Description.SubPixels);
				}
				std::unique_lock<std::mutex> lock(finishCountMutex);
				finished++;
				lock.unlock();
			};
			ThreadPool::SubmitTask(proc);
			submitted++;
		}
	}


	ThreadPool::WaitUntilNoTask();
	ThreadPool::Stop();


	//Save to file
	std::cout << std::endl <<"Saving to " << m_Description.SavePathFileName << std::endl;
	std::ofstream filestream(m_Description.SavePathFileName, std::ios::trunc);
	filestream << "P3" << std::endl << width << " " << height << std::endl << "255" << std::endl;
	for (size_t i = 0; i < ((uint32_t)width) * height; i++)
	{
		filestream << (int)(bitmap[i].r * 255) << " " << (int)(bitmap[i].g * 255) << " " << (int)(bitmap[i].b * 255) << " ";
	}
	filestream.close();
}


//Return the color should be blent
glm::vec3 Renderer::Radiance(const Ray& inRay, uint16_t currentDepth)
{
	/*Hanldle depth*/
	//If depth is deep enough
	if (currentDepth > m_Description.MaxDepth) return { 0.0f,0.0f,0.0f };

	/*Hanldle hitting or not*/
	//Recorder for the nearest hitting object
	float distanceFromRayOrigin = FLT_MAX;
	size_t intersectingObjectID;
	bool hit = false;//We may compare distance with the max to judge, but we need to reduce comparisions

	//Find the nearest intersecting object
	size_t size = m_SceneBuffer.size();
	for (size_t i = 0; i < size; i++)
	{
		auto& geo = m_SceneBuffer[i].geometry;

		float d = geo->Intersect(inRay);
		//If hit
		if (d > 0 && d < distanceFromRayOrigin)
		{
			hit = true;
			distanceFromRayOrigin = d;
			intersectingObjectID = i;
		}
	}
	//Return black(background color) if not hit
	if (!hit) return { 0.0f,0.0f,0.0f };

	/*Handle color or ray interaction*/
	auto& object = m_SceneBuffer[intersectingObjectID];
	auto hitPoint = inRay.origin + distanceFromRayOrigin * inRay.direction;
	auto normal = object.geometry->GetNormal(hitPoint);

	//Generate outRay
	std::vector<Material::WeightedDirection> outDirs;
	auto color = object.material->RayInteraction(inRay.direction, normal, hitPoint, currentDepth, outDirs);
	auto radiantColor = glm::vec3{ 0,0,0 };

	for (auto& outDir : outDirs)
	{
		Ray outRay = Ray(hitPoint, outDir.Direction);
		//Ray interact with the material surface
		radiantColor += outDir.Weight * Radiance(outRay, currentDepth + 1);
	}

	//Calculate final color
	return object.material->emission + color * radiantColor;


}