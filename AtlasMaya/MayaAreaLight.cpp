#include "pch.h"
#include "MayaAreaLight.h"

#include <Maya/MFrameContext.h>
#include <Maya/MDrawContext.h>
#include <Maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnNumericAttribute.h>

#include "MayaNodeId.h"

const MString AreaLightNode::name("AtlasAreaLight");
const MTypeId AreaLightNode::id(MayaNodeId::AREA_LIGHT);

MObject AreaLightNode::aColor;
MObject AreaLightNode::aIntensity;
MObject AreaLightNode::aExposure;
MObject AreaLightNode::aSpread;

void *AreaLightNode::creator()
{
	return (new AreaLightNode);
}

MStatus AreaLightNode::initialize()
{
    MFnNumericAttribute nAttr;

    aColor = nAttr.createColor("color", "cl");
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setReadable(true));
    CHECK_MSTATUS(nAttr.setWritable(true));
    CHECK_MSTATUS(nAttr.setAffectsAppearance(true));
    nAttr.isUsedAsColor();
    CHECK_MSTATUS(nAttr.setDefault(1.f, 1.f, 1.f));

    aIntensity = nAttr.create("intensity", "i", MFnNumericData::kFloat);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setReadable(true));
    CHECK_MSTATUS(nAttr.setWritable(true));
    CHECK_MSTATUS(nAttr.setAffectsAppearance(true));
    CHECK_MSTATUS(nAttr.setDefault(1.0f));

    aExposure = nAttr.create("exposure", "exp", MFnNumericData::kFloat);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setReadable(true));
    CHECK_MSTATUS(nAttr.setWritable(true));
    CHECK_MSTATUS(nAttr.setAffectsAppearance(true));
    CHECK_MSTATUS(nAttr.setDefault(0.0f));
    
    aSpread = nAttr.create("spread", "sp", MFnNumericData::kFloat);
    CHECK_MSTATUS(nAttr.setKeyable(true));
    CHECK_MSTATUS(nAttr.setStorable(true));
    CHECK_MSTATUS(nAttr.setReadable(true));
    CHECK_MSTATUS(nAttr.setWritable(true));
    CHECK_MSTATUS(nAttr.setAffectsAppearance(true));
    CHECK_MSTATUS(nAttr.setDefault(0.0f));

    CHECK_MSTATUS(addAttribute(aColor));
    CHECK_MSTATUS(addAttribute(aIntensity));
    CHECK_MSTATUS(addAttribute(aExposure));
    CHECK_MSTATUS(addAttribute(aSpread));

    return (MS::kSuccess);
}

MStatus AreaLightNode::compute(const MPlug &plug, MDataBlock &block)
{
    color = block.inputValue(aColor).asFloatVector();
    intensity = block.inputValue(aIntensity).asFloat();
    exposure = block.inputValue(aExposure).asFloat();
    spread = block.inputValue(aSpread).asFloat();

    std::cout << "Color: " << color << " intensity: " << intensity << " exposure: " << exposure << " spread: " << spread << std::endl;
    return (MS::kSuccess);
}

bool AreaLightNode::evalNodeAffectsDrawDb(const MEvaluationNode &evaluationNode)
{
    return (true);
}
//
//bool AreaLightNode::isBounded() const
//{
//    return (true);
//};
//
//MBoundingBox AreaLightNode::boundingBox() const
//{
//    return (MBoundingBox(MFloatVector(-1, -1, -1), MFloatVector(1, 1, 1)));
//}

bool AreaLightNode::match(const MSelectionMask &mask, const MObjectArray &componentList) const
{
    if (mask.intersects(MSelectionMask::kSelectLights))
    {
        return (true);
    }
    return (false);
}

MSelectionMask AreaLightNode::getShapeSelectionMask()	const
{
    MSelectionMask mask;
    mask.addMask(MSelectionMask::kSelectLights);
    mask.addMask(MSelectionMask::kSelectLocators);
    return (mask);
}

void AreaLightNode::fetchAttribute(MFnDependencyNode &plug, atlas::AreaLight &light)
{
    MStatus status;

    MFloatVector color(0, 0, 0);
    float intensity = 0;
    float exposure = 0;
    float spread = 0;

    //baseColor.x = plug.findPlug(aBaseColorR, false, &status).asFloat();
    //CHECK_MSTATUS(status);

    //baseColor.y = plug.findPlug(aBaseColorG, false, &status).asFloat();
    //CHECK_MSTATUS(status);

    //baseColor.z = plug.findPlug(aBaseColorB, false, &status).asFloat();
    //CHECK_MSTATUS(status);

    std::cout << plug.findPlug(aColor, false, &status).isArray() << std::endl;

    MPlug plugColor = plug.findPlug(aColor, false, &status);
    CHECK_MSTATUS(status);
    color.x = plugColor.child(0).asFloat();
    color.y = plugColor.child(1).asFloat();
    color.z = plugColor.child(2).asFloat();

    intensity = plug.findPlug(aIntensity, false, &status).asFloat();
    CHECK_MSTATUS(status);

    exposure = plug.findPlug(aExposure, false, &status).asFloat();
    CHECK_MSTATUS(status);

    spread = plug.findPlug(aSpread, false, &status).asFloat();
    CHECK_MSTATUS(status);

    light.color = glm::vec3(color.x, color.y, color.z);
    light.intensity = intensity;
    light.exposure = exposure;
    light.spread = spread;

    std::cout << "Color: " << color << " intensity: " << intensity << " exposure: " << exposure << " spread: " << spread << std::endl;
}

MHWRender::MPxGeometryOverride *AreaLightGeometry::creator(const MObject &obj)
{
    return new AreaLightGeometry(obj);
}

AreaLightGeometry::AreaLightGeometry(const MObject &obj)
    : MHWRender::MPxGeometryOverride(obj)
{
    //MStatus status;
    //MFnDependencyNode node(obj, &status);
    //mNode = status ? dynamic_cast<apiCustomDirectionalLightShape*>(node.userNode()) : NULL;
}

AreaLightGeometry::~AreaLightGeometry()
{
    //mNode = NULL;
}

MHWRender::DrawAPI AreaLightGeometry::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

bool AreaLightGeometry::isIndexingDirty(const MHWRender::MRenderItem &item)
{
    return false;
}

bool AreaLightGeometry::isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc)
{
    return false;
}

void AreaLightGeometry::updateDG()
{
}

void AreaLightGeometry::updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList &list)
{
}

void AreaLightGeometry::populateGeometry(const MHWRender::MGeometryRequirements &requirements, const MHWRender::MRenderItemList &renderItems, MHWRender::MGeometry &data)
{
}

bool AreaLightGeometry::hasUIDrawables() const
{
    return true;
}

void AreaLightGeometry::addUIDrawables(const MDagPath &path, MHWRender::MUIDrawManager &drawManager, const MHWRender::MFrameContext &frameContext)
{
    MStatus status;
    MDagPath objectTransformPath(path);

    objectTransformPath.pop();
    MDagPath cameraPath = frameContext.getCurrentCameraPath();
    cameraPath.pop();
    if (objectTransformPath == cameraPath)
    {
        return;
    }

    drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, 20);

    MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(path);
    if (displayStatus == MHWRender::kDormant)
    {
        drawManager.setColor(MColor(0.39f, 0.40f, 0.38f));
    }
    else
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
        drawManager.setColor(color);
    }

    drawManager.setPaintStyle(MHWRender::MUIDrawManager::kFlat);

    drawManager.setLineWidth(1);
    drawManager.line(MPoint(1, 1, 0), MPoint(-1, 1, 0));
    drawManager.line(MPoint(-1, 1, 0), MPoint(-1, -1, 0));
    drawManager.line(MPoint(-1, -1, 0), MPoint(1, -1, 0));
    drawManager.line(MPoint(1, -1, 0), MPoint(1, 1, 0));
    drawManager.line(MPoint(-1, -1, 0), MPoint(1, 1, 0));
    drawManager.line(MPoint(1, -1, 0), MPoint(-1, 1, 0));
    drawManager.line(MPoint(0, 0, 0), MPoint(0, 0, 1));
    drawManager.endDrawable();
}

void AreaLightGeometry::cleanUp()
{
}