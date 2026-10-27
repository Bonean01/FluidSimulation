using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class LevelSetFreeSurfaceFluidSimulation : FluidSimulation {
    [DllImport("FluidSolver")] private extern static IntPtr GetLevelSetPtr(IntPtr handle);
    public LevelSetFreeSurfaceFluidSimulation(FluidSimulationConfig config) : base(config, SimulationType.LevelSetFreeSurface) {}
    public IEnumerable<float> LevelSetValues() {
        IntPtr ptr = GetLevelSetPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++) {
            yield return GetElementFromPointer<float>(ptr, i);
        }
    }
}
