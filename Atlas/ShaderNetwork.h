#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "atlas/ShaderGraph.h"
#include "atlas/ShaderNode.h"

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	class ShaderNetwork
	{
	public:
		template <typename T>
		static std::shared_ptr<ShaderNode> NodeCreator()
		{
			return (std::make_shared<T>());
		}

		ATLAS ShaderNode &addNode(std::function<std::shared_ptr<ShaderNode>()> create);
		ATLAS size_t findShaderGraph(const std::string &name);
		ATLAS size_t addShaderGraph(const std::string &name);
		ATLAS ShaderGraph &getShaderGraph(size_t idx);
		ATLAS glm::vec3 sampleGraph(size_t idx);

	private:
		std::vector<std::shared_ptr<ShaderNode>> nodes;
		std::vector<ShaderGraph> graph;
	};
}