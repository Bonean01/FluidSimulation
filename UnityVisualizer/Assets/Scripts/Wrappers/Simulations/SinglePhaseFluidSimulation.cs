using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class SinglePhaseFluidSimulation : FluidSimulation {
    public SinglePhaseFluidSimulation(FluidSimulationConfig config) : base(config, SimulationType.SinglePhase) {}
}
