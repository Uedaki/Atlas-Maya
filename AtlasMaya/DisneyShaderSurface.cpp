#include "pch.h"
#include "DisneyShaderSurface.h"

MHWRender::MPxSurfaceShadingNodeOverride *DisneyShaderSurface::creator(const MObject &obj)
{
	return (new DisneyShaderSurface(obj));
}

DisneyShaderSurface::DisneyShaderSurface(const MObject &obj)
	: MPxSurfaceShadingNodeOverride(obj)
{}

DisneyShaderSurface::~DisneyShaderSurface()
{}

MHWRender::DrawAPI DisneyShaderSurface::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}

MString DisneyShaderSurface::fragmentName() const
{
	return ("mayaPhongSurface");
}

void DisneyShaderSurface::getCustomMappings(MHWRender::MAttributeParameterMappingList &mappings)
{
	MHWRender::MAttributeParameterMapping diffuseMapping(
		"color", "outColor", true, true);
	mappings.append(diffuseMapping);
}

MString DisneyShaderSurface::primaryColorParameter() const
{
	return ("outColor");
}

MString DisneyShaderSurface::bumpAttribute() const
{
	return ("normalCamera");
}