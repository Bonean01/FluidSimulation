#pragma once

#include "FreeSurfaceFluidSimulation.h"
#include "math/operators/FiniteDifference.h"

class PLICFreeSurfaceFluidSimulation : public FreeSurfaceFluidSimulation {
public:
	PLICFreeSurfaceFluidSimulation(const FluidSimulationConfig& config) :
		FreeSurfaceFluidSimulation(config),
		m_fluidInterface(m_gridWidth, m_gridHeight),
		m_volumeFractionField(m_gridWidth, m_gridHeight, m_cellWidth),
		m_surfaceCurvatureField(m_gridWidth, m_gridHeight, m_cellWidth) {}

    const FluidInterface2D& getFluidInterface() const { return m_fluidInterface; }
	const ScalarField2D& getVolumeFractionField() const { return m_volumeFractionField; }
	const ScalarField2D& getSurfaceCurvatureField() const { return m_surfaceCurvatureField; }


	void step(float timeStep) override {
		FreeSurfaceFluidSimulation::step(timeStep);
		FiniteDifference::curvature(m_surfaceCurvatureField, m_fluidInterface);
	}


    void setCell(int i, int j, const CellConfig& config) override {
		FreeSurfaceFluidSimulation::setCell(i, j, config);
		if (config.cellData.cellType == CellType::Fluid) m_volumeFractionField.setValue(i, j, 1.0f);
	}


private:
	void updateCellData() override {
		m_domain.updateCellData(m_volumeFractionField);
	}

    void advectProperties(float timeStep) override {
		FreeSurfaceFluidSimulation::advectProperties(timeStep);
		m_advection.execute(m_volumeFractionField, m_fluidInterface, m_velocityField, m_domain, timeStep);
    }

	void updateVelocityField() override {
		FreeSurfaceFluidSimulation::updateVelocityField();

		// Set non-fluid edges to 0.0f
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

	void pressureSolve(float timeStep) override {
		m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_volumeFractionField, m_domain, m_density, timeStep, m_iterationCount);
	}

	//void project(float timeStep) override {
	//	m_projection.execute(m_velocityField, m_pressureField, m_volumeFractionField, m_domain, m_density, timeStep);
	//}

	FluidInterface2D m_fluidInterface;
	ScalarField2D m_volumeFractionField, m_surfaceCurvatureField;
};
