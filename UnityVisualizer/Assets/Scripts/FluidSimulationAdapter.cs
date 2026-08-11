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
        SetCellData();
        unsafe {
            print(sizeof(CellData));
            print(sizeof(BoundaryData));
        }
    }


    private void SetCellData() {
        BoundaryData movingWall = new(BoundaryType.NoSlip, new(10.0f, 0.0f));
        BoundaryData inlet = new(BoundaryType.VelocityInlet, new(10.0f, 0.0f));
        BoundaryData outlet = new(BoundaryType.VelocityOutlet);
        BoundaryData staticWall = new(BoundaryType.NoSlip, new(0.0f, 0.0f));
        for (int i = 0; i < m_width; i++) {
            m_simulation.SetCellData(i, 0, new(CellType.Solid), staticWall);
            m_simulation.SetCellData(i, m_height - 1, new(CellType.Solid), movingWall);
        }
        
        for (int j = 1; j < m_height - 1; j++) {
            m_simulation.SetCellData(0, j, new(CellType.Solid), staticWall);
            m_simulation.SetCellData(m_width - 1, j, new(CellType.Solid), staticWall);
        }

        //Vector2Int origin = new(m_width / 2 + 5, m_height / 2);
        //for (int i = 0; i < m_width; i++) {
        //    for (int j = 0; j < m_height; j++) {
        //        Vector2Int pos = new(i, j);
        //        if ((origin - pos).magnitude < 5) {
        //            m_simulation.SetSolidCell(i, j, true);
        //        }
        //    }
        //}
    }


    public void StepSimulation(float dt) {
        //ApplyVerticalVelocityStream(50.0f);
        //ApplyVelocityImpulse(new(5, 5), new(0.0f, 10.0f), 10);
        //for (int j = 33 / 2 - 5; j <= 33 / 2 + 5; j++) {
        //    m_simulation.AddSmoke(1, j, 0.01f);
        //}
        
        m_simulation.Step(dt);
        OnStateUpdated?.Invoke();
        print(m_simulation.GetVelocity(0, 0).x);
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


    public void ApplySmokeCircle(Vector2 position, float smokeValue, float radius) {
        for (int i = 0; i < m_width; i++) {
            for (int j = 0; j < m_height; j++) {
                if (Mathf.Abs(i - position.x) > radius) continue;
                if (Mathf.Abs(j - position.y) > radius) continue;

                float xDiff = i - position.x;
                float yDiff = j - position.y;
                float dst = Mathf.Sqrt(xDiff * xDiff + yDiff * yDiff);

                if (dst > radius) continue;

                float t = 1 - dst / radius;
                float res = Mathf.SmoothStep(0, smokeValue, t);

                m_simulation.AddSmoke(i, j, res);
            }
        }
        OnStateUpdated?.Invoke();
    }


    public void ApplyVerticalVelocityStream(float value) {
        for (int j = 0; j < m_height; j++) {
            m_simulation.SetVelocity(1, j, new(value, 0.0f));
            m_simulation.SetVelocity(m_width - 1, j, new(value, 0.0f));
        }
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

    /*Maybe we could instead have the first two color channels be for the velocity vector, the third one for the pressure and the last one for some extra
    property such as temperature, then if we need fluid color we can dedicate another texture for that instead of having one texture per property.*/


    public Texture2D CreateTexture() {
        Texture2D res = new(m_width, m_height, TextureFormat.RGBAFloat, mipChain: false, linear: true) {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
        return res;
    }


    private (float min, float max, float total) UpdateScalarFieldTexture(ref Texture2D texture, IEnumerable<float> scalarCollection) {
        int x = 0, y = 0;
        float total = 0;
        float min = float.MaxValue, max = float.MinValue;

        // Traverse the collection such that it gets arranged in a grid that aligns with the texture
        foreach (float value in scalarCollection) {
            if (y >= m_height)
                throw new Exception("The number of values is greater than the number of cells in the simulation");

            Color color = new (value, 0.0f, 0.0f, 1.0f);
            texture.SetPixel(x, y, color);
            
            if (min > value) min = value;
            if (max < value) max = value;
            total += value;

            if (++x >= m_width) { x = 0; y++; }
        }
        texture.Apply();
        return (min, max, total);
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
        //UpdateScalarFieldTexture(ref solidMapCellTexture, m_simulation.CellDataValues());
    }

    public (float min, float max) UpdatePressureTexture(ref Texture2D pressureTexture) {
        var res =  UpdateScalarFieldTexture(ref pressureTexture, m_simulation.PressureValues());
        return (res.min, res.max);
    }

    public (float min, float max) UpdateDivergenceTexture(ref Texture2D divergenceTexture) {
        var res = UpdateScalarFieldTexture(ref divergenceTexture, m_simulation.DivergenceValues());
        return (res.min, res.max);
    }

    public float UpdateSmokeTexture(ref Texture2D smokeTexture) {
        return UpdateScalarFieldTexture(ref smokeTexture, m_simulation.SmokeValues()).total;
    }
}
