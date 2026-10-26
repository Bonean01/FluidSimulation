using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using UnityEngine;

public class SmokeSimulation : FluidSimulation {
    [DllImport("FluidSolver")] private extern static IntPtr CreateSmokeSimulation(FluidSimulationConfig config);
    
    [DllImport("FluidSolver")] private extern static IntPtr GetSmokeFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static void AddSmoke(IntPtr handle, int i, int j, float deltaSmoke);

    public SmokeSimulation(FluidSimulationConfig config) : base(config, SimulationType.Smoke) {}

    public IEnumerable<float> SmokeValues() {
        IntPtr ptr = GetSmokeFieldPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++) {
            yield return GetElementFromPointer<float>(ptr, i);
        }
    }

    public void AddSmoke(int i, int j, float deltaSmoke) => AddSmoke(m_handle, i, j, deltaSmoke);
}
