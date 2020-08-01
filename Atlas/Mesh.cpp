#include "pch.h"
#include "Mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/ext.hpp>

#include <algorithm>
#include <map>

#include "HitRecord.h"
#include "Ray.h"

bool atlas::Mesh::hit(const Ray &ray, float min, float max, HitRecord &record) const
{
	if (!intersectRay(ray, min, max))
		return (false);

	bool hasHit = false;
	for (auto &indice : indices)
	{
		glm::vec3 e1 = vertices[indice.b] - vertices[indice.a];
		glm::vec3 e2 = vertices[indice.c] - vertices[indice.a];
		glm::vec3 normal = glm::cross(e1, e2);
		if (glm::dot(ray.direction, normal) > 0)
			continue;

		float t;
		glm::vec2 coord;
		if (glm::intersectRayTriangle(ray.origin, ray.direction, vertices[indice.a], vertices[indice.b], vertices[indice.c], coord, t))
		{
			if (min + 0.0003 < t && t < max - 0.0003)
			{
				record.distance = t;
				record.position = vertices[indice.a] + coord.x * e1 + coord.y * e2;

				glm::vec3 n1 = normals[indice.nb] - normals[indice.na];
				glm::vec3 n2 = normals[indice.nc] - normals[indice.na];
				record.normal = glm::normalize(normals[indice.na] + coord.x * n1 + coord.y * n2);

				record.material = material;
				max = t;
				hasHit = true;
			}
		}
	}
	return (hasHit);
}