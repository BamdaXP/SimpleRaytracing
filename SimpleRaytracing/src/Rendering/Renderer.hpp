#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Object.hpp"
#include "Camera.hpp"
#include "Geometry/Ray.hpp"

#define PI 3.1415926535897f

struct RendererDescription
{
public:
	uint16_t Width;
	uint16_t Height;
	uint16_t SubPixels;
	uint16_t MaxDepth;
	uint32_t SamplesPerPixel;
	std::string SavePathFileName;
public:
	RendererDescription() = default;
	RendererDescription(uint16_t w, uint16_t h, uint16_t sub, uint16_t maxDepth, uint32_t samples,const std::string& filename)
		:Width(w),Height(h),SubPixels(sub),MaxDepth(maxDepth),SamplesPerPixel(samples),SavePathFileName(filename){}
};
class Renderer 
{
public:
	Renderer(const RendererDescription& desc):m_Description(desc){}
public:
	void Render(const Camera& camera, const std::vector<Object>& scene);

	inline const RendererDescription GetDescription() { return m_Description; }
	inline void SetRendererDescription(const RendererDescription& desc) { m_Description = desc; }
private:
	std::vector<Object> m_SceneBuffer;

	RendererDescription m_Description;
private:
	glm::vec3 Radiance(const Ray& inRay,uint16_t currentDepth);
};