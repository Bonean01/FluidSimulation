using System;
using System.Collections.Generic;
using UnityEngine;

public class FluidSimulationAdapter : MonoBehaviour {
    [SerializeField] private int width, height;
    [SerializeField] private float cellWidth, density, kinematicViscosity;
    [SerializeField] private uint solverIterationCount;

    private int m_width, m_height;
    private FluidSimulation m_simulation;
    public event Action OnStateUpdated;

    public int Width => m_width;
    public int Height => m_height;
    

    private void Awake() {
        m_width = width;
        m_height = height;
        m_simulation = new(m_width, m_height, cellWidth, density, kinematicViscosity, solverIterationCount);
        //DrawSolidCells();
    }


    private void DrawSolidCells() {
        Vector2Int origin = new(m_width / 2 + 5, m_height / 2);
        for (int i = 0; i < m_width; i++) {
            for (int j = 0; j < m_height; j++) {
                Vector2Int pos = new(i, j);
                if ((origin - pos).magnitude < 5) {
                    m_simulation.SetSolidCell(i, j, true);
                }
            }
        }
    }


    public void StepSimulation(float dt) {
        m_simulation.Step(dt);
        if (OnStateUpdated == null) print("bruh");
        OnStateUpdated?.Invoke();
    }


    public void ApplyVelocityImpulse(Vector2 position, Vec2f velocity, float radius) {
        for (int i = 0; i < m_width; i++) {
            for (int j = 0; j < m_height; j++) {
                if (Mathf.Abs(i - position.x) > radius) continue;
                if (Mathf.Abs(j - position.y) > radius) continue;

                float xDiff = i - position.x;
                float yDiff = j - position.y;
                float dst = Mathf.Sqrt(xDiff * xDiff + yDiff * yDiff);

                if (dst > radius) continue;

                float t = 1 - dst / radius;
                float resX = Mathf.SmoothStep(0, velocity.x, t);
                float resY = Mathf.SmoothStep(0, velocity.y, t);

                m_simulation.AddVelocity(i, j, new(resX, resY));
            }
        }
        OnStateUpdated?.Invoke();
    }


    public Vector2Int WorldToGridPoint(Vector2 worldPos) {
        float widthWorld = transform.localScale.x;
        float cellWidthWorld = widthWorld / m_width;
        return new() {
            x = (int)(worldPos.x / cellWidthWorld + m_width / 2),
            y = (int)(worldPos.y / cellWidthWorld + m_height / 2)
        };
    }





    // ##########################################
    //              TEXTURE HANDLING
    // ##########################################


    private Texture2D GetTextureFromPointer(IntPtr ptr, int size) {
        //return Texture2D.CreateExternalTexture()
        throw new NotImplementedException("Would be cool huh");
    }


    /*Maybe we could instead have the first two color channels be for the velocity vector, the third one for the pressure and the last one for some extra
    property such as temperature, then if we need fluid color we can dedicate another texture for that instead of having one texture per property.*/


    public Texture2D CreateTexture() {
        Texture2D res = new(m_width, m_height, TextureFormat.RGBAFloat, mipChain: false, linear: true) {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
        return res;
    }

    private void UpdateVectorFieldTexture(ref Texture2D texture, IEnumerable<Vec2f> vectorCollection) {
        int x = 0, y = 0;
        foreach (Vec2f vec in vectorCollection) {
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
        for (int i = 0; i < m_width; i++) {
            for (int j = 0; j < m_height; j++) {
                Vec2f vel = m_simulation.GetVelocity(i, j);
                Color color = new(vel.x, vel.y, 0.0f, 1.0f);
                velocityTexture.SetPixel(i, j, color);
            }
        }
        velocityTexture.Apply();
    }
    

    public void UpdateSolidMapCellTexture(ref Texture2D solidMapCellTexture) {
        UpdateSolidMapCellTexture(ref solidMapCellTexture, m_simulation.IsSolidCellValues());
    }

    public void UpdatePressureTexture(ref Texture2D pressureTexture) {
        UpdateScalarFieldTexture(ref pressureTexture, m_simulation.PressureValues());
    }
}
