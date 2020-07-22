#include "pch.h"

#include <maya/MDrawRegistry.h>
#include <Maya/MFnPlugin.h>
#include <Maya/MGlobal.h>
#include <maya/MIOStream.h>

#include "DisneyShaderNode.h"
#include "DisneyShaderSurface.h"
#include "RenderProcedure.h"

#include "Trash.h"

namespace
{
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
		plugin.deregisterNode(Node::id);
		MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(sDrawDBClassification, Node::name);
	}
}

MStatus initializePlugin(MObject obj)
{
    std::cout << "Start" << std::endl;
	MStatus status;
	MFnPlugin plugin(obj, "Atlas", "0.1", "any", &status);
	if (status != MS::kSuccess)
	{
		CHECK_MSTATUS(status);
		return (status);
	}
    std::cout << "--------> 1" << std::endl;
	CHECK_MSTATUS(plugin.registerCommand(RenderProcedure::name, RenderProcedure::creator, RenderProcedure::createSyntax));
    std::cout << "---------> 2" << std::endl;
	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya"));
	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya.register; AtlasMaya.register.register()"));
    std::cout << "-------> 3" << std::endl;
	registerMaterial<DisneyShaderNode, DisneyShaderSurface>(plugin);

    MStatus stat = plugin.registerShape("apiPointLightShape", apiPointLightShape::id,
        &apiPointLightShape::creator,
        &apiPointLightShape::initialize,
        NULL,
        &apiPointLightShape::drawdbIdentfier());
    if (!stat) {
        cerr << "Failed to register apiPointLightShape\n";
    }
    stat = plugin.registerShape("apiDirectionalLightShape", apiDirectionalLightShape::id,
        &apiDirectionalLightShape::creator,
        &apiDirectionalLightShape::initialize,
        NULL,
        &apiDirectionalLightShape::drawdbIdentfier());
    if (!stat) {
        cerr << "Failed to register apiDirectionalLightShape\n";
    }
    stat = plugin.registerShape("apiSpotLightShape", apiSpotLightShape::id,
        &apiSpotLightShape::creator,
        &apiSpotLightShape::initialize,
        NULL,
        &apiSpotLightShape::drawdbIdentfier());
    if (!stat) {
        cerr << "Failed to register apiSpotLightShape\n";
    }
    stat = plugin.registerShape("apiAreaLightShape", apiAreaLightShape::id,
        &apiAreaLightShape::creator,
        &apiAreaLightShape::initialize,
        NULL,
        &apiAreaLightShape::drawdbIdentfier());
    if (!stat) {
        cerr << "Failed to register apiAreaLightShape\n";
    }
    stat = plugin.registerShape("apiImageLightShape", apiImageLightShape::id,
        &apiImageLightShape::creator,
        &apiImageLightShape::initialize,
        NULL,
        &apiImageLightShape::drawdbIdentfier());
    if (!stat) {
        cerr << "Failed to register apiImageLightShape\n";
    }
    //
    // Note: We add use "drawdb/geometry/light/directionalLightCustom" to match
    // one of the classifications used for apiCustomDirectionalLightShape.
    // 
    stat = MHWRender::MDrawRegistry::registerGeometryOverrideCreator("drawdb/geometry/light/directionalLightCustom",
        "directionalLightRegistrantId",
        apiDirectionalLightShape_GeometryOverride::Creator);
    if (!stat) {
        cerr << "Failed to register apiDirectionalLightShape_GeometryOverride\n";
        return stat;
    }
    //
    // Note: apiCustomDirectionalLightShape drawdbIdentifier() returns both the lighting and
    // the geometry override classification: "drawdb/geometry/light/directionalLightCustom"
    // allowing for the custom geometry override to replace the default light drawing.
    //
    stat = plugin.registerShape("apiCustomDirectionalLightShape", apiCustomDirectionalLightShape::id,
        &apiCustomDirectionalLightShape::creator,
        &apiCustomDirectionalLightShape::initialize,
        NULL,
        &apiCustomDirectionalLightShape::drawdbIdentfier());
    if (!stat) {
        cerr << "Failed to register apiCustomDirectionalLightShape\n";
    }

	std::cout << "Module Loaded" << std::endl;
	return (MS::kSuccess);
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

    MStatus stat;
    stat = plugin.deregisterNode(apiPointLightShape::id);
    if (!stat) {
        cerr << "Failed to deregister apiPointLightShape\n";
    }
    stat = plugin.deregisterNode(apiDirectionalLightShape::id);
    if (!stat) {
        cerr << "Failed to deregister apiDirectionalLightShape\n";
    }
    stat = plugin.deregisterNode(apiSpotLightShape::id);
    if (!stat) {
        cerr << "Failed to deregister apiSpotLightShape\n";
    }
    stat = plugin.deregisterNode(apiAreaLightShape::id);
    if (!stat) {
        cerr << "Failed to deregister apiAreaLightShape\n";
    }
    stat = plugin.deregisterNode(apiImageLightShape::id);
    if (!stat) {
        cerr << "Failed to deregister apiImageLightShape\n";
    }
    stat = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator("drawdb/geometry/light/directionalLightCustom",
        "directionalLightRegistrantId");
    if (!stat) {
        cerr << "Failed to deregister apiDirectionalLightShape_GeometryOverride\n";
    }
    stat = plugin.deregisterNode(apiCustomDirectionalLightShape::id);
    if (!stat) {
        cerr << "Failed to deregister apiCustomDirectionalLightShape\n";
    }
    
    
    CHECK_MSTATUS(plugin.deregisterCommand(RenderProcedure::name));

	deregisterMaterial<DisneyShaderNode, DisneyShaderSurface>(plugin);

	CHECK_MSTATUS(MGlobal::executePythonCommand("import AtlasMaya.register; AtlasMaya.register.unregister()"));

    std::cout << "---------------UNLOADED-----------" << std::endl;

	return (MS::kSuccess);
}