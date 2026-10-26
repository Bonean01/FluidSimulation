#pragma once

#include "FluidSimulation.h"
#include "steps/ExternalForces.h"

class FreeSurfaceFluidSimulation : public FluidSimulation {
public:
	FreeSurfaceFluidSimulation(const FluidSimulationConfig& config)
		: FluidSimulation(config),
		m_fluidLevelSet(m_gridWidth, m_gridHeight),
		m_fluidInterface(m_gridWidth, m_gridHeight),
		m_volumeFractionField(m_gridWidth, m_gridHeight, m_cellWidth) {}

	const LevelSet2D& getLevelSet() const { return m_fluidLevelSet; }
	const FluidInterface2D& getFluidInterface() const { return m_fluidInterface; }
	const ScalarField2D& getVolumeFractionField() const { return m_volumeFractionField; }


	void step(float timeStep) override {
		// TODO: Select depending on the volume tracking method used
		//m_fluidSurface.update(m_markerParticles, 5);
		//m_domain.extrapolateVelocity(m_velocityField, m_fluidSurface);
		m_advection.execute(m_volumeFractionField, m_fluidInterface, m_velocityField, m_domain, timeStep);
		m_domain.updateCellData(m_volumeFractionField);

		m_advection.execute(m_markerParticles, m_velocityField, m_domain, timeStep);

		if (m_applyGravity) ExternalForces::applyGravity(m_velocityField, m_domain, m_volumeFractionField, timeStep);

		m_domain.applyVelocityBoundaryConditions(m_velocityField);

		m_advection.execute(m_velocityField, m_domain, timeStep);
		m_diffusion.execute(m_velocityField, m_domain, m_kinematicViscosity, timeStep, m_iterationCount);
		m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_domain, m_density, timeStep, m_iterationCount);
		m_projection.execute(m_velocityField, m_pressureField, m_domain, m_density, timeStep);

		Staggered::divergence(m_divergenceField, m_velocityField);
	}

	void setCell(int i, int j, const CellConfig& config) override {
		FluidSimulation::setCell(i, j, config);
		if (config.cellData.cellType == CellType::Fluid) m_volumeFractionField.setValue(i, j, 1.0f);
	}


private:
	LevelSet2D m_fluidLevelSet;
	FluidInterface2D m_fluidInterface;
	ScalarField2D m_volumeFractionField;
};
