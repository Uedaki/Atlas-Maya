#pragma once

#include <map>

#include "atlas/ShaderAttribute.h"
#include "atlas/ShaderNetwork.h"
#include "atlas/ShaderNodeBuilder.h"

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	class ShaderNetworkBuilder
	{
	public:
		ATLAS				ShaderNetworkBuilder(ShaderNetwork &shaderNetwork);
		ATLAS ShaderNode	&addNode(const std::string &name, std::function<std::shared_ptr<ShaderNode>()> creator);
		ATLAS void			connect(const std::string &srcNode, const std::string &srcAttrName, const std::string &dstNode, const std::string &dstAttrName);
		ATLAS void			connectToGraph(size_t graphIdx, const std::string &srcNode, const std::string &srcAttrName);

		ATLAS bool			doesNodeExist(const std::string &name) const;

	protected:
		atlas::ShaderNetwork						&network;
		std::map<std::string, ShaderNodeBuilder>	nodes;
	};
}