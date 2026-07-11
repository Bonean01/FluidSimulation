using UnityEngine;

public class PluginUser : MonoBehaviour {
    private void Awake() {
        try {
            FluidSimulation simulation = new();
            
            print(simulation.IsRunning());
            simulation.Stop();
            print(simulation.IsRunning());

        } catch (System.Exception e) {
            print(e);            
        }
    }
}
