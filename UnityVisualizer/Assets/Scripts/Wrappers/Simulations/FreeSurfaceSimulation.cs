using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class FreeSurfaceSimulation : FluidSimulation {
    [DllImport("FluidSolver")] private extern static IntPtr GetLevelSetPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static IntPtr GetFluidInterfacePtr(IntPtr handle);

    public FreeSurfaceSimulation(FluidSimulationConfig config) : base(config, SimulationType.FreeSurface) {}

    public IEnumerable<float> LevelSetValues() {
        IntPtr ptr = GetLevelSetPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++) {
            yield return GetElementFromPointer<float>(ptr, i);
        }
    }

    public IEnumerable<InterfaceData> InterfaceValues() {
        IntPtr ptr = GetFluidInterfacePtr(m_handle);
        for (int i = 0; i < m_cellCount; i++) {
            yield return GetElementFromPointer<InterfaceData>(ptr, i);
        }
    }
}
