using UnityEngine;

public class PluginUser : MonoBehaviour {
    [SerializeField] private int width, height;
    [SerializeField] private float cellWidth;
    private void Awake() {
        FluidSimulation simulation = new(width, height, cellWidth);
        for (int i = 0; i < simulation.GetWidth(); i++) {
            for (int j = 0; j < simulation.GetHeight(); j++) {
                float x = Random.Range(-10.0f, 10.0f);
                float y = Random.Range(-10.0f, 10.0f);
                simulation.SetVelocity(i, j, new(x, y));
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
    }
}
