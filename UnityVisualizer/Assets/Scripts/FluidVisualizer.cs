using UnityEngine;

public class PluginUser : MonoBehaviour {
    [SerializeField] private int width, height;
    [SerializeField] private float cellWidth;
    private void Awake() {
        try {
            FluidSimulation simulation = new(width, height, cellWidth);
            foreach (Vector2 velocity in simulation.VelocityVectors()) {
                print($"({velocity.x}, {velocity.y})");
            }
        } catch (System.Exception e) {
            print(e);            
        }
    }
}
