#pragma once

#include <map>
#include <string>

#include "atlas/ShaderAttribute.h"
#include "atlas/ShaderNetwork.h"

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	class ShaderNodeBuilder
	{
	public:
		ATLAS					ShaderNodeBuilder(ShaderNode &owner);
		ATLAS void				registerAttribute(const std::string &name, ShaderAttribute &attribute);

		ATLAS ShaderAttribute	*getAttribute(const std::string &name);

		inline ShaderNode		&getOwner() { return (owner); }
		inline const ShaderNode &getOwner() const { return (owner); }

	private:
		ShaderNode									&owner;
		std::map<std::string, ShaderAttribute &>	attributes;
	};
}