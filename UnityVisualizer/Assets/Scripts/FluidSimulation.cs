using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class FluidSimulation : IDisposable {
    readonly IntPtr handle;
    [DllImport("FluidSolver")] private extern static IntPtr CreateSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity);
    [DllImport("FluidSolver")] private extern static IntPtr CreateSimulation(int width, int height, float cellWidth);
    [DllImport("FluidSolver")] private extern static void DestroySimulation(IntPtr handle);

    [DllImport("FluidSolver")] private extern static void Step(IntPtr handle, float dt);

    [DllImport("FluidSolver")] private extern static IntPtr GetVelocityFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static int GetVelocityFieldSize(IntPtr handle);


    public FluidSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity) {
        handle = CreateSimulation(width, height, cellWidth, density, kinematicViscosity);
    }
    public FluidSimulation(int width, int height, float cellWidth) {
        handle = CreateSimulation(width, height, cellWidth);
    }
    public void Dispose() {
        DestroySimulation(handle);
    }

    public void Step(float dt) {
        Step(handle, dt);
    }

    public IEnumerable<Vector2> VelocityVectors() {
        IntPtr ptr = GetVelocityFieldPtr(handle);
        int size = GetVelocityFieldSize(handle);
        for (int i = 0; i < size; i++)
            yield return GetElementFromPointer<Vector2>(ptr, i);
    }

    unsafe private T GetElementFromPointer<T>(IntPtr ptr, int index) where T : unmanaged {
        return ((T*)ptr)[index];
    }
}
