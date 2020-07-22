#include "pch.h"
#include "ShaderNetworkBuilder.h"

atlas::ShaderNetworkBuilder::ShaderNetworkBuilder(ShaderNetwork &shaderNetwork)
	: network(shaderNetwork)
{}

atlas::ShaderNode &atlas::ShaderNetworkBuilder::addNode(const std::string &name, std::function<std::shared_ptr<ShaderNode>()> creator)
{
	ShaderNode &node = network.addNode(creator);
	nodes.emplace(name, node);
	return (node);
}

void atlas::ShaderNetworkBuilder::connect(const std::string &srcNode, const std::string &srcAttrName, const std::string &dstNode, const std::string &dstAttrName)
{
	if (nodes.find(srcNode) == nodes.end() || nodes.find(dstNode) == nodes.end())
	{
		return;
	}

	ShaderNodeBuilder &src = nodes.at(srcNode);
	ShaderNodeBuilder &dst = nodes.at(dstNode);

	const ShaderAttribute *srcAttr = src.getAttribute(srcAttrName);
	ShaderAttribute *dstAttr = dst.getAttribute(srcAttrName);

	if (!srcAttr || !dstAttr)
		return;

	dst.getOwner().addDependency(src.getOwner());
	dstAttr->connect(srcAttr);
}

bool atlas::ShaderNetworkBuilder::doesNodeExist(const std::string &name) const
{
	return (nodes.find(name) != nodes.end());
}

void atlas::ShaderNetworkBuilder::connectToGraph(size_t graphIdx, const std::string &srcNode, const std::string &srcAttrName)
{
	if (nodes.find(srcNode) == nodes.end())
		return;
	ShaderNodeBuilder &src = nodes.at(srcNode);
	const ShaderAttribute *srcAttr = src.getAttribute(srcAttrName);

	if (!srcAttr)
		return;

	network.getShaderGraph(graphIdx).connect(src.getOwner(), *srcAttr);
}