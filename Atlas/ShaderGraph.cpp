#include "pch.h"
#include "ShaderGraph.h"

atlas::ShaderGraph::ShaderGraph(const std::string &name)
	: name(name)
{}

void atlas::ShaderGraph::connect(ShaderNode &node, const ShaderAttribute &source)
{
	dependency = &node;
	surface.connect(&source);
}

glm::vec3 atlas::ShaderGraph::sample()
{
	if (dependency)
		dependency->sample();
	return (surface.getValue());
}