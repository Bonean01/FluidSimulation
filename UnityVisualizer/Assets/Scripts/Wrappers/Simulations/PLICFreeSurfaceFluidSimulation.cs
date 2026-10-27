using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class PLICFreeSurfaceFluidSimulation : FluidSimulation {
    [DllImport("FluidSolver")] private extern static IntPtr GetFluidInterfacePtr(IntPtr handle);
    public PLICFreeSurfaceFluidSimulation(FluidSimulationConfig config) : base(config, SimulationType.PLICFreeSurface) {}
    public IEnumerable<InterfaceData> InterfaceValues() {
        IntPtr ptr = GetFluidInterfacePtr(m_handle);
        for (int i = 0; i < m_cellCount; i++) {
            yield return GetElementFromPointer<InterfaceData>(ptr, i);
        }
    }
}
