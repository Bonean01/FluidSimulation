using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class FluidSimulation : IDisposable {
    readonly IntPtr m_handle;
    readonly int m_width, m_height, m_cellCount;


    public int GetWidth() => m_width;
    public int GetHeight() => m_height;
    public int GetCellCount() => m_cellCount;


    [DllImport("FluidSolver")] private extern static IntPtr CreateSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity);
    [DllImport("FluidSolver")] private extern static void DestroySimulation(IntPtr handle);

    [DllImport("FluidSolver")] private extern static void Step(IntPtr handle, float dt);

    [DllImport("FluidSolver")] private extern static IntPtr GetVelocityFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static IntPtr GetPressureFieldPtr(IntPtr handle);
    [DllImport("FluidSolver")] private extern static IntPtr GetSolidCellMapPtr(IntPtr handle);

    [DllImport("FluidSolver")] private extern static void SetVelocity(IntPtr handle, int i, int j, Vector2 velocity);
    [DllImport("FluidSolver")] private extern static void SetSolidCell(IntPtr handle, int i, int j, bool isSolid);


    public FluidSimulation(int width, int height, float cellWidth, float density = 1, float kinematicViscosity = 0) {
        m_handle = CreateSimulation(width, height, cellWidth, density, kinematicViscosity);
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

    public IEnumerable<Vector2> VelocityVectors() {
        IntPtr ptr = GetVelocityFieldPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<Vector2>(ptr, i);
    }

    public IEnumerable<float> PressureValues() {
        IntPtr ptr = GetPressureFieldPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<float>(ptr, i);
    }

    public IEnumerable<byte> IsSolidCellValues() {
        IntPtr ptr = GetSolidCellMapPtr(m_handle);
        for (int i = 0; i < m_cellCount; i++)
            yield return GetElementFromPointer<byte>(ptr, i);
    }


    public void SetVelocity(int i, int j, Vector2 velocity) {
        SetVelocity(m_handle, i, j, velocity);
    }

    public void SetSolidCell(int i, int j, bool isSolid) {
        SetSolidCell(m_handle, i, j, isSolid);
    }


    private Texture2D GetTextureFromPointer(IntPtr ptr, int size) {
        //return Texture2D.CreateExternalTexture()
        throw new NotImplementedException("Would be cool huh");
    }



    /*Maybe we could instead have the first two color channels be for the velocity vector, the third one for the pressure and the last one for some extra
    property such as temperature, then if we need fluid color we can dedicate another texture for that instead of having one texture per property.*/




    private void UpdateVectorFieldTexture(ref Texture2D texture, IEnumerable<Vector2> vectorCollection) {
        int x = 0, y = 0;
        foreach (Vector2 vec in vectorCollection) {
            if (y >= m_height) throw new Exception("The number of values is greater than the number of cells in the simulation");
            Color color = new (vec.x, vec.y, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            if (++x >= m_width) { x = 0; y++; }
        }
        texture.Apply();
    }

    private void UpdateScalarFieldTexture(ref Texture2D texture, IEnumerable<float> scalarCollection) {
        int x = 0, y = 0;
        foreach (float value in scalarCollection) {
            if (y >= m_height) throw new Exception("The number of values is greater than the number of cells in the simulation");
            Color color = new (value, 0.0f, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            if (++x >= m_width) { x = 0; y++; }
        }
        texture.Apply();
    }

    // TODO: fix this mess
    private void UpdateSolidMapCellTexture(ref Texture2D texture, IEnumerable<byte> scalarCollection) {
        int x = 0, y = 0;
        foreach (byte value in scalarCollection) {
            if (y >= m_height) throw new Exception("The number of values is greater than the number of cells in the simulation");
            Color color = new (value, 0.0f, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            if (++x >= m_width) { x = 0; y++; }
        }
        texture.Apply();
    }

    public void UpdateVelocityTexture(ref Texture2D velocityTexture) {
        UpdateVectorFieldTexture(ref velocityTexture, VelocityVectors());
    }

    public void UpdateSolidMapCellTexture(ref Texture2D solidMapCellTexture) {
        UpdateSolidMapCellTexture(ref solidMapCellTexture, IsSolidCellValues());
    }

    public void UpdatePressureTexture(ref Texture2D pressureTexture) {
        UpdateScalarFieldTexture(ref pressureTexture, PressureValues());
    }
}
