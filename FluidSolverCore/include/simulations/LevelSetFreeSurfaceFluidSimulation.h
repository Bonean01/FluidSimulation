#pragma once

#include "FreeSurfaceFluidSimulation.h"

class LevelSetFreeSurfaceFluidSimulation : public FreeSurfaceFluidSimulation {
public:
    LevelSetFreeSurfaceFluidSimulation(const FluidSimulationConfig& config) :
        FreeSurfaceFluidSimulation(config),
        m_fluidLevelSet(m_gridWidth, m_gridHeight) {}

    const LevelSet2D& getLevelSet() const { return m_fluidLevelSet; }

    void step(float time) override {
        m_fluidLevelSet.update(m_markerParticles, 5);
		m_domain.extrapolateVelocity(m_velocityField, m_fluidLevelSet);
        m_domain.updateCellData(m_markerParticles, m_fluidLevelSet);
        FreeSurfaceFluidSimulation::step(time);
    }

private:
    LevelSet2D m_fluidLevelSet;
};
