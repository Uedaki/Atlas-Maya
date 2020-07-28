#pragma once

#include <maya/MMessage.h>
#include <maya/MTypeId.h> 
#include <maya/MPxComponentShape.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MDrawRegistry.h>
#include <maya/MHWGeometryUtilities.h>

#include <Maya/MPxLocatorNode.h>
#include <Maya/MUserData.h>
#include <maya/MPxDrawOverride.h>

#include "Atlas/AreaLight.h"

// Light parameter
//
// - color
// - intensity
// - exposure ? no sure
// - spread

class AreaLightNode : public MPxSurfaceShape
{
public:
    static const MString name;
    static const MTypeId id;

    static  void *creator();
    static  MStatus initialize();

    AreaLightNode() = default;
    ~AreaLightNode() override = default;
    
    bool evalNodeAffectsDrawDb(const MEvaluationNode &evaluationNode) override;

    //bool isBounded() const override;
    //MBoundingBox boundingBox() const override;

    bool match(const MSelectionMask &mask, const MObjectArray &componentList) const override;
    MSelectionMask getShapeSelectionMask()	const override;

    MStatus compute(const MPlug &plug, MDataBlock &block) override;
    static void fetchAttribute(MFnDependencyNode &plug, atlas::AreaLight &light);

private:
    static MObject aColorR;
    static MObject aColorG;
    static MObject aColorB;
    static MObject aColor;

    static MObject aIntensity;
    static MObject aExposure;

    static MObject aSpread;

    MFloatVector color;
    float intensity;
    float exposure;
    float spread;
};

class AreaLightGeometry : public MHWRender::MPxGeometryOverride
{
public:
    static MHWRender::MPxGeometryOverride *creator(const MObject &obj);

    AreaLightGeometry(const MObject &obj);
    ~AreaLightGeometry() override;

    MHWRender::DrawAPI supportedDrawAPIs() const override;
    
    bool isIndexingDirty(const MHWRender::MRenderItem& item) override;
    bool isStreamDirty(const MHWRender::MVertexBufferDescriptor& desc) override;
    
    void updateDG() override;
    void updateRenderItems(const MDagPath& path, MHWRender::MRenderItemList& list) override;
    
    void populateGeometry(const MHWRender::MGeometryRequirements& requirements, const MHWRender::MRenderItemList& renderItems, MHWRender::MGeometry& data) override;
    
    bool hasUIDrawables() const override;
    void addUIDrawables(const MDagPath& path, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext) override;
    
    void cleanUp() override;
protected:
    //apiCustomDirectionalLightShape* mNode;
};