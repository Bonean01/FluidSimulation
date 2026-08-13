using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class FluidSimulation : IDisposable {
    readonly IntPtr m_handle;
    readonly int m_width, m_height, m_cellCount;


    public int GetWidth() => m_width;
    public int GetHeight() => m_height;
    public int GetCellCount() => m_cellCount;


    [DllImport("FluidSolver")] private extern static IntPtr CreateSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity, uint iterationCount);
    [DllImport("FluidSolver")] private extern static void DestroySimulation(IntPtr handle);

    [DllImport("FluidSolver")] private extern static void Step(IntPtr handle, float dt);

    [DllImport("FluidSolver")] private extern static IntPtr GetPressureFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static IntPtr GetDivergenceFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static IntPtr GetSmokeFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static IntPtr GetCellDataPtr(IntPtr handle);

    [DllImport("FluidSolver")] private extern static void SetVelocity(IntPtr handle, int i, int j, Vec2f velocity);
    [DllImport("FluidSolver")] private extern static Vec2f GetVelocity(IntPtr handle, int i, int j);
    [DllImport("FluidSolver")] private extern static void AddVelocity(IntPtr handle, int i, int j, Vec2f deltaVel);
    [DllImport("FluidSolver")] private extern static void AddSmoke(IntPtr handle, int i, int j, float deltaSmoke);
    [DllImport("FluidSolver")] private extern static void SetCellData(IntPtr handle, int i, int j, CellData cellData, BoundaryData boundaryData);


    public FluidSimulation(int width, int height, float cellWidth, float density = 1, float kinematicViscosity = 0, uint iterationCount = 60) {
        m_handle = CreateSimulation(width, height, cellWidth, density, kinematicViscosity, iterationCount);
        m_width = width;
        m_height = height;
        m_cellCount = width * height;
    }

    public void Dispose() {
        DestroySimulation(m_handle);
    }


    public void Step(float dt) {
        Step(m_handle, dt);
    }


    unsafe private T GetElementFromPointer<T>(IntPtr ptr, int index) where T : unmanaged {
        return ((T*)ptr)[index];
    }

    public IEnumerable<float> PressureValues() {
        IntPtr ptr = GetPressureFieldPtr(m_handle); 
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<float>(ptr, i);
    }

    public IEnumerable<float> DivergenceValues() {
        IntPtr ptr = GetDivergenceFieldPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<float>(ptr, i);
    }

    public IEnumerable<float> SmokeValues() {
        IntPtr ptr = GetSmokeFieldPtr(m_handle); 
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<float>(ptr, i);
    }

    public IEnumerable<CellData> CellDataValues() {
        IntPtr ptr = GetCellDataPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<CellData>(ptr, i);
    }


    public void SetVelocity(int i, int j, Vec2f velocity) => SetVelocity(m_handle, i, j, velocity);
    public Vec2f GetVelocity(int i, int j) => GetVelocity(m_handle, i, j);
    public void AddVelocity(int i, int j, Vec2f deltaVel) => AddVelocity(m_handle, i, j, deltaVel);
    public void AddSmoke(int i, int j, float deltaSmoke) => AddSmoke(m_handle, i, j, deltaSmoke);

    public void SetCell(int i, int j, CellData cellData, BoundaryData boundaryData) => SetCellData(m_handle, i, j, cellData, boundaryData);
}
