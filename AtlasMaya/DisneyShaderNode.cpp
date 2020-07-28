#include "pch.h"
#include "DisneyShaderNode.h"

#include <Maya/MFloatVector.h>
#include <Maya/MFnDependencyNode.h>
#include <Maya/MFnNumericAttribute.h>

#include "Atlas/DisneyShader.h"

#include "MayaNodeId.h"

const MString DisneyShaderNode::name("AtlasDisneyMaterial");
const MTypeId DisneyShaderNode::id(MayaNodeId::SHADER_DISNEY);

MObject  DisneyShaderNode::aBaseColorR;
MObject  DisneyShaderNode::aBaseColorG;
MObject  DisneyShaderNode::aBaseColorB;
MObject  DisneyShaderNode::aBaseColor;

MObject  DisneyShaderNode::aOutColorR;
MObject  DisneyShaderNode::aOutColorG;
MObject  DisneyShaderNode::aOutColorB;
MObject  DisneyShaderNode::aOutColor;

MObject  DisneyShaderNode::aSubsurface;
MObject  DisneyShaderNode::aMetallic;
MObject  DisneyShaderNode::aSpecular;
MObject  DisneyShaderNode::aSpecularTint;
MObject  DisneyShaderNode::aRoughness;
MObject  DisneyShaderNode::aAnisotropic;
MObject  DisneyShaderNode::aSheen;
MObject  DisneyShaderNode::aSheenTint;
MObject  DisneyShaderNode::aClearCoat;
MObject  DisneyShaderNode::aClearCoatGloss;

void *DisneyShaderNode::creator()
{
	return (new DisneyShaderNode);
}

MStatus DisneyShaderNode::initialize()
{
	MStatus status;
	MFnNumericAttribute nAttr;

	{
		aBaseColorR = nAttr.create("baseColorR", "bcr", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.0f));
		aBaseColorG = nAttr.create("baseColorG", "bcg", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.58824f));
		aBaseColorB = nAttr.create("baseColorB", "bcb", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.644f));
		aBaseColor = nAttr.create("baseColor", "bc", aBaseColorR, aBaseColorG, aBaseColorB, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.0f, 0.58824f, 0.644f));
		CHECK_MSTATUS(nAttr.setUsedAsColor(true));
	}

	{
		aOutColorR = nAttr.create("outColorR", "ocr", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		aOutColorG = nAttr.create("outColorG", "ocg", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		aOutColorB = nAttr.create("outColorB", "ocb", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		aOutColor = nAttr.create("outColor", "oc", aOutColorR, aOutColorG, aOutColorB, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setHidden(false));
		CHECK_MSTATUS(nAttr.setReadable(true));
		CHECK_MSTATUS(nAttr.setWritable(false));
	}

	{
		aSubsurface = nAttr.create("subsurface", "ss", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aMetallic = nAttr.create("metallic", "m", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aSpecular = nAttr.create("specular", "s", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.5f));
	}

	{
		aSpecularTint = nAttr.create("specularTint", "st", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aRoughness = nAttr.create("roughness", "r", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.4f));
	}

	{
		aAnisotropic = nAttr.create("anisotropic", "a", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aSheen = nAttr.create("sheen", "se", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aSheenTint = nAttr.create("sheenTint", "set", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aClearCoat = nAttr.create("clearCoat", "cc", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		aClearCoatGloss = nAttr.create("clearCoatGloss", "ccg", MFnNumericData::kFloat, 0, &status);
		CHECK_MSTATUS(status);
		CHECK_MSTATUS(nAttr.setReadable(false));
		CHECK_MSTATUS(nAttr.setKeyable(true));
		CHECK_MSTATUS(nAttr.setStorable(true));
		CHECK_MSTATUS(nAttr.setDefault(0.f));
	}

	{
		CHECK_MSTATUS(addAttribute(aBaseColor));
		CHECK_MSTATUS(addAttribute(aOutColor));
		CHECK_MSTATUS(addAttribute(aSubsurface));
		CHECK_MSTATUS(addAttribute(aMetallic));
		CHECK_MSTATUS(addAttribute(aSpecular));
		CHECK_MSTATUS(addAttribute(aSpecularTint));
		CHECK_MSTATUS(addAttribute(aRoughness));
		CHECK_MSTATUS(addAttribute(aAnisotropic));
		CHECK_MSTATUS(addAttribute(aSheen));
		CHECK_MSTATUS(addAttribute(aSheenTint));
		CHECK_MSTATUS(addAttribute(aClearCoat));
		CHECK_MSTATUS(addAttribute(aClearCoatGloss));
	}

	{
		CHECK_MSTATUS(attributeAffects(aBaseColorR, aOutColor));
		CHECK_MSTATUS(attributeAffects(aBaseColorG, aOutColor));
		CHECK_MSTATUS(attributeAffects(aBaseColorB, aOutColor));
		CHECK_MSTATUS(attributeAffects(aBaseColor, aOutColor));
		CHECK_MSTATUS(attributeAffects(aSubsurface, aOutColor));
		CHECK_MSTATUS(attributeAffects(aMetallic, aOutColor));
		CHECK_MSTATUS(attributeAffects(aSpecularTint, aOutColor));
		CHECK_MSTATUS(attributeAffects(aRoughness, aOutColor));
		CHECK_MSTATUS(attributeAffects(aAnisotropic, aOutColor));
		CHECK_MSTATUS(attributeAffects(aSheen, aOutColor));
		CHECK_MSTATUS(attributeAffects(aSheenTint, aOutColor));
		CHECK_MSTATUS(attributeAffects(aClearCoat, aOutColor));
		CHECK_MSTATUS(attributeAffects(aClearCoatGloss, aOutColor));
	}

	return (MS::kSuccess);
}

void DisneyShaderNode::fetchAttribute(MFnDependencyNode &plug, atlas::ShaderNode &node)
{
	MStatus status;
	atlas::DisneyShader *shader;
	if (!(shader = dynamic_cast<atlas::DisneyShader *>(&node)))
		return;

	MFloatVector baseColor(0, 0, 0);
	float subsurface = 0;
	float metallic = 0;
	float specular = 0;
	float specularTint = 0;
	float roughness = 0;
	float anisotropic = 0;
	float sheen = 0;
	float sheenTint = 0;
	float clearCoat = 0;
	float clearCoatGloss = 0;

	baseColor.x = plug.findPlug(aBaseColorR, false, &status).asFloat();
	CHECK_MSTATUS(status);

	baseColor.y = plug.findPlug(aBaseColorG, false, &status).asFloat();
	CHECK_MSTATUS(status);

	baseColor.z = plug.findPlug(aBaseColorB, false, &status).asFloat();
	CHECK_MSTATUS(status);

	subsurface = plug.findPlug(aSubsurface, false, &status).asFloat();
	CHECK_MSTATUS(status);

	metallic = plug.findPlug(aMetallic, false, &status).asFloat();
	CHECK_MSTATUS(status);

	specular = plug.findPlug(aSpecular, false, &status).asFloat();
	CHECK_MSTATUS(status);

	specularTint = plug.findPlug(aSpecularTint, false, &status).asFloat();
	CHECK_MSTATUS(status);

	roughness = plug.findPlug(aRoughness, false, &status).asFloat();
	CHECK_MSTATUS(status);

	anisotropic = plug.findPlug(aAnisotropic, false, &status).asFloat();
	CHECK_MSTATUS(status);

	sheen = plug.findPlug(aSheen, false, &status).asFloat();
	CHECK_MSTATUS(status);

	sheenTint = plug.findPlug(aSheenTint, false, &status).asFloat();
	CHECK_MSTATUS(status);

	clearCoat = plug.findPlug(aClearCoat, false, &status).asFloat();
	CHECK_MSTATUS(status);

	clearCoatGloss = plug.findPlug(aClearCoatGloss, false, &status).asFloat();
	CHECK_MSTATUS(status);

	shader->baseColor.setFloat3(baseColor.x, baseColor.y, baseColor.z);
	shader->subsurface.setFloat(subsurface);
	shader->metallic.setFloat(metallic);
	shader->specular.setFloat(specular);
	shader->specularTint.setFloat(specularTint);
	shader->roughness.setFloat(roughness);
	shader->anisotropic.setFloat(anisotropic);
	shader->sheen.setFloat(sheen);
	shader->sheenTint.setFloat(sheenTint);
	shader->clearCoat.setFloat(clearCoat);
	shader->clearCoatGloss.setFloat(clearCoatGloss);
}

MStatus DisneyShaderNode::compute(const MPlug &plug, MDataBlock &block)
{
	MStatus status;

	MFloatVector color(0, 0, 0);

	MFloatVector baseColor(0, 0, 0);
	float subsurface = 0;
	float metallic = 0;
	float specular = 0;
	float specularTint = 0;
	float roughness = 0;
	float anisotropic = 0;
	float sheen = 0;
	float sheenTint = 0;
	float clearCoat = 0;
	float clearCoatGloss = 0;

	{
		baseColor = block.inputValue(aBaseColor, &status).asFloatVector();
		CHECK_MSTATUS(status);

		subsurface = block.inputValue(aSubsurface, &status).asFloat();
		CHECK_MSTATUS(status);

		metallic = block.inputValue(aMetallic, &status).asFloat();
		CHECK_MSTATUS(status);

		specular = block.inputValue(aSpecular, &status).asFloat();
		CHECK_MSTATUS(status);

		specularTint = block.inputValue(aSpecularTint, &status).asFloat();
		CHECK_MSTATUS(status);

		roughness = block.inputValue(aRoughness, &status).asFloat();
		CHECK_MSTATUS(status);

		anisotropic = block.inputValue(aAnisotropic, &status).asFloat();
		CHECK_MSTATUS(status);

		sheen = block.inputValue(aSheen, &status).asFloat();
		CHECK_MSTATUS(status);

		sheenTint = block.inputValue(aSheenTint, &status).asFloat();
		CHECK_MSTATUS(status);

		clearCoat = block.inputValue(aClearCoat, &status).asFloat();
		CHECK_MSTATUS(status);

		clearCoatGloss = block.inputValue(aClearCoatGloss, &status).asFloat();
		CHECK_MSTATUS(status);
	}

	MDataHandle outColorHandle = block.outputValue(aOutColor, &status);
	MFloatVector &outColor = outColorHandle.asFloatVector();
	outColor = baseColor;
	outColorHandle.setClean();

	return(MS::kSuccess);
}