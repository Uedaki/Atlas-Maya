#include "pch.h"
#include "ShaderAttribute.h"

float atlas::ShaderFloatAttribute::getValue() const
{
	if (source)
		return (source->asFloat());
	return (value);
}

glm::vec3 atlas::ShaderColorAttribute::getValue() const
{
	glm::vec3 color(r.getValue(), g.getValue(), b.getValue());

	if (source)
		color = source->asFloat3();
	return (color);
}

void atlas::ShaderColorAttribute::operator=(const ShaderColorAttribute &attr)
{
	glm::vec3 color = attr.getValue();
	r.setFloat(color.x);
	g.setFloat(color.y);
	b.setFloat(color.z);
}

void atlas::ShaderColorAttribute::setFloat3(float x, float y, float z)
{
	r.setFloat(x);
	g.setFloat(y);
	b.setFloat(z);
}