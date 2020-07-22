#pragma once

#include "atlas/ShaderAttribute.h"
#include "atlas/ShaderNode.h"
#include "atlas/ShaderNodeBuilder.h"

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	struct ShaderConstantFloat : public ShaderNode
	{
		ShaderFloatAttribute outFloat;

		ATLAS void initBuilder(ShaderNodeBuilder &builder) override;
		ATLAS void sample() override;
	};

	struct ShaderConstantColor : public ShaderNode
	{
		ShaderColorAttribute outColor;

		ATLAS void initBuilder(ShaderNodeBuilder &builder) override;
		ATLAS void sample() override;
	};
}