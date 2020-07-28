#pragma once

#include <cstdint>

enum MayaNodeId : uint32_t
{
	FIRST_LIGHT = 0xff700,
		AREA_LIGHT = FIRST_LIGHT,
	LAST_LIGHT,

	FIRST_SHADER,
		SHADER_DISNEY = FIRST_SHADER,
	LAST_SHADER
};