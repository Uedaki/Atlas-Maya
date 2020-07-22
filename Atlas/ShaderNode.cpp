#include "pch.h"
#include "ShaderNode.h"

void atlas::ShaderNode::sample()
{
	for (auto &dependency : dependencies)
	{
		dependency->sample();
	}
}

void atlas::ShaderNode::addDependency(ShaderNode &node)
{
	dependencies.emplace_back(&node);
}