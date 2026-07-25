#include "math/dataStructures/MACGrid.h"

int main(int argc, char* argv[]) {
	MACGrid2D grid{ 2, 2, 1.0f };
	grid.setEdgeX(0, 0, 1.0f);
	grid.setEdgeY(0, 0, 1.0f);
	Vec2f res = grid.sampleBilinear(0, 0);
	// TODO: add extra stuff once we have chatch2
}
