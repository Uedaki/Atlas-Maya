#pragma once

#include <Maya/MPxNode.h>

#include "Atlas/ShaderNode.h"

class DisneyShaderNode : public MPxNode
{
public:
	static const MString name;
	static const MTypeId id;

	static void *creator();
	static MStatus initialize();

	static void fetchAttribute(MFnDependencyNode &plug, atlas::ShaderNode &node);

	MStatus compute(const MPlug &plug, MDataBlock &block) override;

private:
	static MObject  aBaseColorR;
	static MObject  aBaseColorG;
	static MObject  aBaseColorB;
	static MObject  aBaseColor;

	static MObject  aOutColorR;
	static MObject  aOutColorG;
	static MObject  aOutColorB;
	static MObject  aOutColor;

	static MObject  aSubsurface;
	static MObject  aMetallic;
	static MObject  aSpecular;
	static MObject  aSpecularTint;
	static MObject  aRoughness;
	static MObject  aAnisotropic;
	static MObject  aSheen;
	static MObject  aSheenTint;
	static MObject  aClearCoat;
	static MObject  aClearCoatGloss;
};