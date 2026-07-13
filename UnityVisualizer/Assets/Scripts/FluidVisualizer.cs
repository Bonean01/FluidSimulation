using UnityEngine;


[RequireComponent(typeof(SpriteRenderer))]
public class FluidVisualizer : MonoBehaviour {
    [SerializeField] private int width, height;
    [SerializeField] private float cellWidth;
    private Texture2D velocityTexture, pressureTexture, solidCellMapTexture;
    
    private void Awake() {
        FluidSimulation simulation = new(width, height, cellWidth);
        for (int i = 0; i < simulation.GetWidth(); i++) {
            for (int j = 0; j < simulation.GetHeight(); j++) {
                float velX = Random.Range(0.0f, 1.0f);
                float velY = Random.Range(0.0f, 1.0f);
                simulation.SetVelocity(i, j, new(velX, velY));
            }
        }

        int index = 0;
        foreach (Vector2 velocity in simulation.VelocityVectors()) {
            print($"{index++} \t Velocity: ({velocity.x}, {velocity.y})");
        }
        index = 0;
        foreach (float pressure in simulation.PressureValues()) {
            print($"{index++} \t Pressure: {pressure}");
        }
        index = 0;
        foreach (byte isSolid in simulation.IsSolidCellValues()) {
            print($"{index++} \t IsSolid: {isSolid == 1}");
        }

        // Create the textures from the fields
        velocityTexture = new (width, height, TextureFormat.RGBA32, mipChain:false, linear:true);
        int x = 0, y = 0;
        foreach (Vector2 velocity in simulation.VelocityVectors()) {
            if (y >= height) throw new System.Exception("More velocity vectors that expected for this texture");
            Color color = new(velocity.x, velocity.y, 0, 1);
            velocityTexture.SetPixel(x, y, color);
            if (++x >= width) { x = 0; y++; }
        }
        velocityTexture.filterMode = FilterMode.Point;
        velocityTexture.wrapMode = TextureWrapMode.Clamp;
        velocityTexture.Apply();

        GetComponent<SpriteRenderer>().material.SetTexture("_VelocityTexture", velocityTexture);
    }
}
