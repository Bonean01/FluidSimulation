#pragma once

#include "FluidSimulation.h"


class SmokeSimulation : public FluidSimulation {
public:
    SmokeSimulation(const FluidSimulationConfig& config) :
        FluidSimulation(config),
        m_smokeField(m_gridWidth, m_gridHeight, m_cellWidth) { m_domain.flood(); }

	const ScalarField2D& getSmokeField() const { return m_smokeField; }

    void step(float timeStep) override {
        FluidSimulation::step(timeStep);
        m_advection.execute(m_smokeField, m_velocityField, m_domain, timeStep);
    }
	void addSmoke(int i, int j, float deltaSmoke) { m_smokeField.setValue(i, j, m_smokeField.getValue(i, j) + deltaSmoke); }


private:
    ScalarField2D m_smokeField;
};
