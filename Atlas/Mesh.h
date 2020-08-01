#pragma once

#include <vector>

#include "Hitable.h"

namespace atlas
{
	struct Ray;
	struct HitRecord;

	class Mesh : public Hitable
	{
		struct Triangle
		{
			int a;
			int na;
			int b;
			int nb;
			int c;
			int nc;
		};

	public:
		bool hit(const Ray &ray, float min, float max, HitRecord &record) const;

	protected:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<Triangle> indices;
		uint32_t material;
	};
}