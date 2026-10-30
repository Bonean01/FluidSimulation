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
		if (m_applyGravity) ExternalForces::applyGravity(m_velocityField, m_domain, m_volumeFractionField, timeStep);
        FreeSurfaceFluidSimulation::step(timeStep);
		m_advection.execute(m_volumeFractionField, m_fluidInterface, m_velocityField, m_domain, timeStep);
    }

    void setCell(int i, int j, const CellConfig& config) override {
		FluidSimulation::setCell(i, j, config);
		if (config.cellData.cellType == CellType::Fluid) m_volumeFractionField.setValue(i, j, 1.0f);
	}


private:
	void updateCellData() override { m_domain.updateCellData(m_volumeFractionField); }

	FluidInterface2D m_fluidInterface;
	ScalarField2D m_volumeFractionField;
};
