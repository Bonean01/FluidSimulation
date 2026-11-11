#pragma once

#include "FreeSurfaceFluidSimulation.h"

class LevelSetFreeSurfaceFluidSimulation : public FreeSurfaceFluidSimulation {
public:
    LevelSetFreeSurfaceFluidSimulation(const FluidSimulationConfig& config) :
        FreeSurfaceFluidSimulation(config),
        m_fluidLevelSet(m_gridWidth, m_gridHeight) {}

    const LevelSet2D& getLevelSet() const { return m_fluidLevelSet; }


private:
    void updateCellData() override {
        m_fluidLevelSet.update(m_markerParticles, 5);
        m_domain.updateCellData(m_markerParticles, m_fluidLevelSet);
    }
    void updateVelocityField() override {
        m_domain.extrapolateVelocity(m_velocityField, m_fluidLevelSet);
        FluidSimulation::updateVelocityField();
    }

    LevelSet2D m_fluidLevelSet;
};
