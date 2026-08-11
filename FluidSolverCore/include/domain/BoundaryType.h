#pragma once

#include <cstdint>

enum class BoundaryType : uint8_t {
	None,
	NoSlip,
	FreeSlip,
	VelocityInlet,
	VelocityOutlet
};
