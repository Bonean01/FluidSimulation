using UnityEngine;
using System.Runtime.InteropServices;

public class PluginUser : MonoBehaviour {
    [DllImport("FluidSolver")]
    extern static uint Echo(uint number);

    private void Awake() {
        try {
            print(Echo(5));
        } catch (System.Exception e) {
            print(e);            
        }
    }
}
