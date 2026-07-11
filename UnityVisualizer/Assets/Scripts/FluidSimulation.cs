using System;
using System.Runtime.InteropServices;

public class FluidSimulation : IDisposable {
    readonly IntPtr handle;
    [DllImport("FluidSolver")] private extern static IntPtr CreateSimulation();
    [DllImport("FluidSolver")] private extern static void DestroySimulation(IntPtr handle);

    public FluidSimulation() {
        handle = CreateSimulation();
    }

    public void Dispose() {
        DestroySimulation(handle);
    }
}
