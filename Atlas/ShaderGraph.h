#pragma once

#include <string>

#include "atlas/ShaderAttribute.h"
#include "atlas/ShaderNode.h"

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	class ShaderGraph
	{
	public:
		ShaderGraph() = default;
		ATLAS ShaderGraph(const std::string &name);

		ATLAS glm::vec3 sample();
		ATLAS void connect(ShaderNode &node, const ShaderAttribute &source);

		inline const std::string getName() const { return (name); }

	private:
		std::string name;

		ShaderNode *dependency = nullptr;
		ShaderColorAttribute surface;
	};
}