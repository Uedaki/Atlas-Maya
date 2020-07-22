#pragma once

#include <glm/glm.hpp>

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
	class DisneyShader : public ShaderNode
	{
	public:
		ShaderColorAttribute outColor;

		ShaderColorAttribute baseColor;
		ShaderFloatAttribute subsurface;
		ShaderFloatAttribute metallic;
		ShaderFloatAttribute specular;
		ShaderFloatAttribute specularTint;
		ShaderFloatAttribute roughness;
		ShaderFloatAttribute anisotropic;
		ShaderFloatAttribute sheen;
		ShaderFloatAttribute sheenTint;
		ShaderFloatAttribute clearCoat;
		ShaderFloatAttribute clearCoatGloss;

		ATLAS void initBuilder(ShaderNodeBuilder &builder) override;
		ATLAS void sample() override;
	};
}