#pragma once

#include <cstdint>

enum class BoundaryCondition : uint8_t {
	None,
	Dirichlet,
	HomogeneousNeumann
};
