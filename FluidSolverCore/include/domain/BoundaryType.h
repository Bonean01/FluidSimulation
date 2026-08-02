#pragma once

#include <cstdint>

enum BoundaryType : uint8_t {
	None,
	NoSlip,
	FreeSlip,
	Inlet,
	Outlet
};
