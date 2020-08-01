#pragma once

#include <glm/glm.hpp>

namespace atlas
{
	struct HitRecord
	{
		float distance;
		glm::vec3 position;
		glm::vec3 normal;
		uint32_t material;
	};
}