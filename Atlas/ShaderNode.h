#pragma once

#include <vector>

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	class ShaderNodeBuilder;

	class ShaderNode
	{
	public:
		virtual void initBuilder(ShaderNodeBuilder &builder) = 0;
		ATLAS virtual void sample();
		ATLAS void addDependency(ShaderNode &node);

	private:
		std::vector<ShaderNode *> dependencies;
	};
}