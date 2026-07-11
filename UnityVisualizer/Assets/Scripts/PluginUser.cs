using UnityEngine;

public class PluginUser : MonoBehaviour {
    private void Awake() {
        try {
            FluidSimulation simulation = new();
            

        } catch (System.Exception e) {
            print(e);            
        }
    }
}
