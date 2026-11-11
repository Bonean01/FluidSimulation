#pragma once

#include "FluidSimulation.h"

class SmokeSimulation : public SinglePhaseFluidSimulation {
public:
    SmokeSimulation(const FluidSimulationConfig& config) :
        SinglePhaseFluidSimulation(config),
        m_smokeField(m_gridWidth, m_gridHeight, m_cellWidth) {}

	const ScalarField2D& getSmokeField() const { return m_smokeField; }

	void addSmoke(int i, int j, float deltaSmoke) { m_smokeField.setValue(i, j, m_smokeField.getValue(i, j) + deltaSmoke); }


private:
    void advectProperties(float timeStep) override {
        FluidSimulation::advectProperties(timeStep);
        m_advection.execute(m_smokeField, m_velocityField, m_domain, timeStep);
    }
    ScalarField2D m_smokeField;
};
