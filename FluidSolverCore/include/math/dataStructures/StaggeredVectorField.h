#pragma once

#include "StaggeredGrid.h"

class StaggeredVectorField2D : public StaggeredGrid2D<float> {
public:
    StaggeredVectorField2D(int width, int height, float cellWidth) : StaggeredGrid2D(width, height, cellWidth) {}
    
    Vec2f sampleBilinear(float x, float y) const;
    Vec2f sampleBilinear(const Vec2f& position) const;

	Vec2f getCellValue(int i, int j) const;
	void setCellValue(int i, int j, Vec2f value);

private:
    float sampleBilinear(const VectorComponent& C, float x, float y) const;
};
