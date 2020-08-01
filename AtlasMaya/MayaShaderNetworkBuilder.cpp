#include "pch.h"
#include "MayaShaderNetworkBuilder.h"

#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include "Atlas/DisneyShader.h"
#include "Atlas/ShaderConstant.h"
#include "Atlas/ShaderNodeBuilder.h"

#include "DisneyShaderNode.h"
#include "Logger.h"

namespace
{
	// color constant 529954
	// float constant 529952
	// atlas disney material 5222322

	void fetchFloatConstant(MFnDependencyNode &plug, atlas::ShaderNode &node)
	{
		MStatus status;
		atlas::ShaderConstantFloat *shader;
		if (!(shader = dynamic_cast<atlas::ShaderConstantFloat *>(&node)))
			return;
		MPlug attr = plug.findPlug("outFloat", false, &status);
		CHECK_MSTATUS(status);

		shader->outFloat.setFloat(attr.asFloat());
	}

	void fetchColorConstant(MFnDependencyNode &plug, atlas::ShaderNode &node)
	{
		MStatus status;
		atlas::ShaderConstantColor *shader;
		if (!(shader = dynamic_cast<atlas::ShaderConstantColor *>(&node)))
			return;

		MFloatVector color;
		color.x = plug.findPlug("outColorR", false, &status).asFloat();
		CHECK_MSTATUS(status);
		color.y = plug.findPlug("outColorG", false, &status).asFloat();
		CHECK_MSTATUS(status);
		color.z = plug.findPlug("outColorB", false, &status).asFloat();
		CHECK_MSTATUS(status);

		shader->outColor.setFloat3(color.x, color.y, color.z);
	}

	struct ShaderNodeFunctions
	{
		std::function<std::shared_ptr<atlas::ShaderNode>()> creator;
		std::function<void(MFnDependencyNode &, atlas::ShaderNode &)> fetch;
	};

	const std::map<uint32_t, ShaderNodeFunctions> link =
	{
		{522232, {atlas::ShaderNetwork::NodeCreator<atlas::DisneyShader>, DisneyShaderNode::fetchAttribute}},
		{529952, {atlas::ShaderNetwork::NodeCreator<atlas::ShaderConstantFloat>, fetchFloatConstant}},
		{529954, {atlas::ShaderNetwork::NodeCreator<atlas::ShaderConstantColor>, fetchColorConstant}}
	};
}

size_t MayaShaderNetworkBuilder::buildGraph(MObject shadingGroupObject)
{
	MFnDependencyNode shadingGroup(shadingGroupObject);

	size_t index;
	if ((index = network.findShaderGraph(shadingGroup.name().asChar())) != -1)
		return (index);

	Stopwatch timer;
	index = network.addShaderGraph(shadingGroup.name().asChar());
	MObject surfaceAttribute = shadingGroup.attribute("surfaceShader");
	MPlug surface(shadingGroupObject, surfaceAttribute);

	MPlug surfaceShader = surface.source();
	if (!surfaceShader.isNull())
	{
		buildConnection(surfaceShader);

		std::string src = surfaceShader.name().asChar();
		std::string srcNode;
		std::string srcAttr;

		{
			size_t i = 0;
			i = src.find_first_of('.');
			if (i == std::string::npos)
				return (index);

			srcNode = src.substr(0, i);
			srcAttr = src.substr(i + 1, src.length() - i - 1);
		}

		connectToGraph(index, srcNode, srcAttr);
		
		timer.stop();
		LOG_INFO("Shading graph \"" << srcNode << "\" created in " << timer.elapsed());
	}

	return (index);
}

void MayaShaderNetworkBuilder::connect(const std::string &src, const std::string &dst)
{
	std::string srcNode;
	std::string srcAttr;

	std::string dstNode;
	std::string dstAttr;

	{
		size_t i = 0;
		i = src.find_first_of('.');
		if (i == std::string::npos)
			return;

		srcNode = src.substr(0, i);
		srcAttr = src.substr(i + 1, src.length() - i - 1);
	}

	{
		size_t i = 0;
		i = dst.find_first_of('.');
		if (i == std::string::npos)
			return;

		dstNode = dst.substr(0, i);
		dstAttr = dst.substr(i + 1, dst.length() - i - 1);
	}

	atlas::ShaderNetworkBuilder::connect(srcNode, srcAttr, dstNode, dstAttr);
}

void MayaShaderNetworkBuilder::buildConnection(MPlug nodePlug)
{
	MStatus status;
	MFnDependencyNode node(nodePlug.node());

	if (doesNodeExist(node.name().asChar()))
		return;

	if (link.find(node.typeId().id()) != link.end())
	{
		ShaderNodeFunctions shaderFunctions = link.at(node.typeId().id());
		
		atlas::ShaderNode &shader = addNode(node.name().asChar(), shaderFunctions.creator);

		shaderFunctions.fetch(node, shader);
	}

	MPlugArray connections;
	node.getConnections(connections);
	for (auto connection : connections)
	{
		if (connection.isDestination())
		{
			MPlug source = connection.source(&status);
			CHECK_MSTATUS(status);

			buildConnection(source);

			connect(source.name().asChar(), connection.name().asChar());
		}
	}
}