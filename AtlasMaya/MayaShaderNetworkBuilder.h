#pragma once

#include "Atlas/ShaderNetworkBuilder.h"

#include <maya/MObject.h>

class MayaShaderNetworkBuilder : public atlas::ShaderNetworkBuilder
{
public:
	MayaShaderNetworkBuilder(atlas::ShaderNetwork &network)
		: atlas::ShaderNetworkBuilder(network)
	{}

	size_t buildGraph(MObject shadingGroupObject);

	void connect(const std::string &src, const std::string &dst);

	void buildConnection(MPlug nodePlug);
};