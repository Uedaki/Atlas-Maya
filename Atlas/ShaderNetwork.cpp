#include "pch.h"
#include "ShaderNetwork.h"

atlas::ShaderNode &atlas::ShaderNetwork::addNode(std::function<std::shared_ptr<ShaderNode>()> create)
{
	std::shared_ptr<ShaderNode> node = create();
	nodes.emplace_back(node);
	return (*node);
}

size_t atlas::ShaderNetwork::findShaderGraph(const std::string &name)
{
	for (size_t i = 0; i < graph.size(); i++)
	{
		if (graph[i].getName() == name)
		{
			return (i);
		}
	}
	return (-1);
}

size_t atlas::ShaderNetwork::addShaderGraph(const std::string &name)
{
	graph.emplace_back(name);
	return (graph.size() - 1);
}

atlas::ShaderGraph &atlas::ShaderNetwork::getShaderGraph(size_t idx)
{
	return (graph[idx]);
}

glm::vec3 atlas::ShaderNetwork::sampleGraph(size_t idx)
{
	return (graph[idx].sample());
}