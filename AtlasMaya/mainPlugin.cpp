#include "pch.h"

#include <maya/MDrawRegistry.h>
#include <Maya/MFnPlugin.h>
#include <Maya/MGlobal.h>
#include <maya/MIOStream.h>

#include "DisneyShaderNode.h"
#include "DisneyShaderSurface.h"
#include "RenderProcedure.h"

#include "Logger.h"
#include "MayaAreaLight.h"

namespace
{
	const MString MayaRegistrantID("AtlasMaya");

	template <typename Node, typename ShadingNode>
	void registerMaterial(MFnPlugin &plugin)
	{
		MString sDrawDBClassification("drawdb/shader/surface/" + Node::name);
		MString sFullClassification("shader/surface:" + sDrawDBClassification);
		CHECK_MSTATUS(plugin.registerNode(Node::name, Node::id, Node::creator, Node::initialize, MPxNode::kDependNode, &sFullClassification));
		CHECK_MSTATUS(MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(sDrawDBClassification, Node::name, ShadingNode::creator));
	}

	template <typename Node, typename ShadingNode>
	void deregisterMaterial(MFnPlugin &plugin)
	{
		MString sDrawDBClassification("drawdb/shader/surface/" + Node::name);
		CHECK_MSTATUS(plugin.deregisterNode(Node::id));
		CHECK_MSTATUS(MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(sDrawDBClassification, Node::name));
	}

	template <typename Node, typename Geometry>
	void registerLight(MFnPlugin &plugin)
	{
		MString sDrawDBClassification("drawdb/geometry/" + Node::name);
		CHECK_MSTATUS(plugin.registerShape(Node::name, Node::id, &Node::creator, &Node::initialize, NULL, &sDrawDBClassification));
		//CHECK_MSTATUS(plugin.registerNode(Node::name, Node::id, Node::creator, Node::initialize, MPxNode::Type::kLocatorNode, &sDrawDBClassification))
		CHECK_MSTATUS(MHWRender::MDrawRegistry::registerGeometryOverrideCreator(sDrawDBClassification, MayaRegistrantID, Geometry::creator));
	}

	template <typename Node, typename Geometry>	
	void deregisterLight(MFnPlugin &plugin)
	{
		MString sDrawDBClassification("drawdb/geometry/" + Node::name);
		CHECK_MSTATUS(plugin.deregisterNode(Node::id));
		CHECK_MSTATUS(MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(sDrawDBClassification, MayaRegistrantID));
	}
}

MStatus initializePlugin(MObject obj)
{
	LOG_INFO("Loading plugin Atlas...");

	MStatus status;
	MFnPlugin plugin(obj, "Atlas", "0.1", "any", &status);
	if (status != MS::kSuccess)
	{
		CHECK_MSTATUS(status);
		return (status);
	}

	CHECK_MSTATUS(plugin.registerCommand(RenderProcedure::name, RenderProcedure::creator, RenderProcedure::createSyntax));

	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya"));
	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya.register; AtlasMaya.register.register()"));
	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya.menu; AtlasMaya.menu.createMenu()"));

	registerMaterial<DisneyShaderNode, DisneyShaderSurface>(plugin);

	registerLight<AreaLightNode, AreaLightGeometry>(plugin);

	LOG_INFO("Plugin loaded successfully");

	return (MS::kSuccess);
}

MStatus uninitializePlugin(MObject obj)
{
	LOG_INFO("Plugin Atlas unloaded");

	MFnPlugin plugin(obj);

    CHECK_MSTATUS(plugin.deregisterCommand(RenderProcedure::name));

	deregisterMaterial<DisneyShaderNode, DisneyShaderSurface>(plugin);

	deregisterLight<AreaLightNode, AreaLightGeometry>(plugin);

	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya.register; AtlasMaya.register.unregister()"));
	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya.menu; AtlasMaya.menu.deleteMenu()"));

	return (MS::kSuccess);
}