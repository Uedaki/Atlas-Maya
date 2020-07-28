#pragma once

#include <glm/glm.hpp>

#include "Atlas/Light.h"

namespace atlas
{
	class AreaLight : public Light
	{
	public:
		glm::vec3 color;
		float intensity;
		float exposure;
		float spread;
	};
}