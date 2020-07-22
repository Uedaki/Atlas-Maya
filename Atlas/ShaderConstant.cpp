#include "pch.h"
#include "ShaderConstant.h"

void atlas::ShaderConstantFloat::initBuilder(ShaderNodeBuilder &builder)
{
	builder.registerAttribute("outFloat", outFloat);
}

void atlas::ShaderConstantFloat::sample()
{
	ShaderNode::sample();
}

void atlas::ShaderConstantColor::initBuilder(ShaderNodeBuilder &builder)
{
	builder.registerAttribute("outColor", outColor);
}

void atlas::ShaderConstantColor::sample()
{
	ShaderNode::sample();
}