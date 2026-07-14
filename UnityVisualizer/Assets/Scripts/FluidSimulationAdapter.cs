using System;
using System.Collections.Generic;
using UnityEngine;

public class FluidSimulationAdapter : MonoBehaviour {
    [SerializeField] private int width, height;
    [SerializeField] private float cellWidth;
    FluidSimulation m_simulation;
    

    private void Awake() {
        m_simulation = new(width, height, cellWidth);

        float aspectRatio = (float)height / width;
        transform.localScale = new Vector3(transform.localScale.x, transform.localScale.x * aspectRatio, transform.localScale.z);
    }


    private void Start() {
         for (int j = 10; j < m_simulation.GetHeight() - 10; j++) {
            m_simulation.SetVelocity(3, j, new(20.0f, 0.0f));
        }
    }


    private Texture2D GetTextureFromPointer(IntPtr ptr, int size) {
        //return Texture2D.CreateExternalTexture()
        throw new NotImplementedException("Would be cool huh");
    }


    /*Maybe we could instead have the first two color channels be for the velocity vector, the third one for the pressure and the last one for some extra
    property such as temperature, then if we need fluid color we can dedicate another texture for that instead of having one texture per property.*/


    public Texture2D CreateTexture() {
        Texture2D res = new(width, height, TextureFormat.RGBA32, mipChain: false, linear: true);
        return res;
    }

    private void UpdateVectorFieldTexture(ref Texture2D texture, IEnumerable<Vector2> vectorCollection) {
        int x = 0, y = 0;
        foreach (Vector2 vec in vectorCollection) {
            if (y >= height) throw new Exception("The number of values is greater than the number of cells in the simulation");
            Color color = new (vec.x, vec.y, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            if (++x >= width) { x = 0; y++; }
        }
        texture.Apply();
    }

    private void UpdateScalarFieldTexture(ref Texture2D texture, IEnumerable<float> scalarCollection) {
        int x = 0, y = 0;
        foreach (float value in scalarCollection) {
            if (y >= height) throw new Exception("The number of values is greater than the number of cells in the simulation");
            Color color = new (value, 0.0f, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            if (++x >= width) { x = 0; y++; }
        }
        texture.Apply();
    }

    // TODO: fix this mess
    private void UpdateSolidMapCellTexture(ref Texture2D texture, IEnumerable<byte> scalarCollection) {
        int x = 0, y = 0;
        foreach (byte value in scalarCollection) {
            if (y >= height) throw new Exception("The number of values is greater than the number of cells in the simulation");
            Color color = new (value, 0.0f, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            if (++x >= width) { x = 0; y++; }
        }
        texture.Apply();
    }

    public void UpdateVelocityTexture(ref Texture2D velocityTexture) {
        UpdateVectorFieldTexture(ref velocityTexture, m_simulation.VelocityVectors());
    }

    public void UpdateSolidMapCellTexture(ref Texture2D solidMapCellTexture) {
        UpdateSolidMapCellTexture(ref solidMapCellTexture, m_simulation.IsSolidCellValues());
    }

    public void UpdatePressureTexture(ref Texture2D pressureTexture) {
        UpdateScalarFieldTexture(ref pressureTexture, m_simulation.PressureValues());
    }
}
