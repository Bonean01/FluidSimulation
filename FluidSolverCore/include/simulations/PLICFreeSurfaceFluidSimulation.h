#pragma once

#include "FreeSurfaceFluidSimulation.h"

class PLICFreeSurfaceFluidSimulation : public FreeSurfaceFluidSimulation {
public:
    PLICFreeSurfaceFluidSimulation(const FluidSimulationConfig& config) :
        FreeSurfaceFluidSimulation(config),
		m_fluidInterface(m_gridWidth, m_gridHeight),
		m_volumeFractionField(m_gridWidth, m_gridHeight, m_cellWidth) {}

    const FluidInterface2D& getFluidInterface() const { return m_fluidInterface; }
	const ScalarField2D& getVolumeFractionField() const { return m_volumeFractionField; }

    void step(float timeStep) override {
        FreeSurfaceFluidSimulation::step(timeStep);
		m_advection.execute(m_volumeFractionField, m_fluidInterface, m_velocityField, m_domain, timeStep);
    }

    void setCell(int i, int j, const CellConfig& config) override {
		FluidSimulation::setCell(i, j, config);
		if (config.cellData.cellType == CellType::Fluid) m_volumeFractionField.setValue(i, j, 1.0f);
	}


private:
	void updateCellData() override { m_domain.updateCellData(m_volumeFractionField); }
	void updateVelocityField() override {
		using enum VectorComponent;
		for (VectorComponent C : { X, Y }) {
			#pragma omp parallel for
			for (int j = 0; j < m_gridHeight; j++) {
				for (int i = 0; i < m_gridWidth; i++) {
					const BoundaryData& currentBoundary = m_domain.getBoundary(C, i, j);
					if (m_domain.hasEdgePrescribedVelocity(C, i, j)) continue;
					if (not m_domain.isFluidEdge(C, i, j)) m_velocityField.setEdgeValue(C, i, j, 0.0f);
				}
			}
		}
	}

	FluidInterface2D m_fluidInterface;
	ScalarField2D m_volumeFractionField;
};
