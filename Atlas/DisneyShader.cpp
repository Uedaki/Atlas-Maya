#include "pch.h"
#include "DisneyShader.h"

void atlas::DisneyShader::initBuilder(ShaderNodeBuilder &builder)
{
	builder.registerAttribute("outColor", outColor);
	builder.registerAttribute("outColorR", outColor.r);
	builder.registerAttribute("outColorG", outColor.g);
	builder.registerAttribute("outColorB", outColor.b);

	builder.registerAttribute("baseColor", baseColor);
	builder.registerAttribute("baseColorR", baseColor.r);
	builder.registerAttribute("baseColorG", baseColor.g);
	builder.registerAttribute("baseColorB", baseColor.b);

	builder.registerAttribute("subsurface", subsurface);
	builder.registerAttribute("metallic", metallic);
	builder.registerAttribute("specular", specular);
	builder.registerAttribute("specularTint", specularTint);
	builder.registerAttribute("roughness", roughness);
	builder.registerAttribute("anisotropic", anisotropic);
	builder.registerAttribute("sheen", sheen);
	builder.registerAttribute("sheenTint", sheenTint);
	builder.registerAttribute("clearCoat", clearCoat);
	builder.registerAttribute("clearCoatGloss", clearCoatGloss);
}

void atlas::DisneyShader::sample()
{
	ShaderNode::sample();
	outColor = baseColor;
}