#include "pch.h"
#include "ShaderNodeBuilder.h"

atlas::ShaderNodeBuilder::ShaderNodeBuilder(ShaderNode &owner)
	: owner(owner)
{
	owner.initBuilder(*this);
}

void atlas::ShaderNodeBuilder::registerAttribute(const std::string &name, ShaderAttribute &attribute)
{
	attributes.emplace(name, attribute);
}

atlas::ShaderAttribute *atlas::ShaderNodeBuilder::getAttribute(const std::string &name)
{
	if (attributes.find(name) != attributes.end())
		return (&attributes.at(name));
	return (nullptr);
}