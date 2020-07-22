#pragma once

#include <glm/glm.hpp>

# ifdef ATLAS_EXPORTS
#   define ATLAS  __declspec( dllexport )
# else
#   define ATLAS __declspec( dllimport )
# endif

namespace atlas
{
	class ShaderAttribute
	{
	public:
		const ShaderAttribute *source = nullptr;

		inline void connect(const ShaderAttribute *node) { source = node; }
		inline bool isConnected() const { return (source); }

		inline virtual float asFloat() const = 0;
		inline virtual glm::vec3 asFloat3() const = 0;
	};

	class ShaderFloatAttribute : public ShaderAttribute
	{
	public:
		ATLAS float getValue() const;

		inline void setFloat(float v) { value = v; };
		inline float asFloat() const override { return (value); }
		inline glm::vec3 asFloat3() const override { return (glm::vec3(value)); }

		inline void operator=(const ShaderFloatAttribute &attr)	{ value = getValue(); }

	private:
		float value = 0.0f;
	};

	class ShaderColorAttribute : public ShaderAttribute
	{
	public:
		ATLAS glm::vec3 getValue() const;

		ATLAS void setFloat3(float x, float y, float z);
		inline float asFloat() const override { return (r.getValue()); }
		inline glm::vec3 asFloat3() const override { return (getValue()); }

		ATLAS void operator=(const ShaderColorAttribute &attr);

		ShaderFloatAttribute r;
		ShaderFloatAttribute g;
		ShaderFloatAttribute b;
	};

}