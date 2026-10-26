using System.Runtime.InteropServices;


public enum LinearSolverAlgorithm {
    JACOBI
}


[StructLayout(LayoutKind.Sequential)]
public struct LinearSolverConfig {
    public LinearSolverAlgorithm linearSolverAlgorithm;
    public uint iterationCount;

    public LinearSolverConfig(LinearSolverAlgorithm linearSolverAlgorithm = LinearSolverAlgorithm.JACOBI, uint iterationCount = 30) {
        this.linearSolverAlgorithm = linearSolverAlgorithm;
        this.iterationCount = iterationCount;
    }
}


[StructLayout(LayoutKind.Sequential)]
public struct FluidSimulationConfig {
    public int gridWidth;
    public int gridHeight;
    public float cellWidth;
    public float density;
    public float kinematicViscosity;
    public LinearSolverConfig linearSolverConfig;
    public uint threadCount;
    public bool applyGravity;

    public FluidSimulationConfig(int gridWidth, int gridHeight, float cellWidth, float density, float kinematicViscosity, LinearSolverConfig linearSolverConfig = new(), uint threadCount = 0, bool applyGravity = false) {
        this.gridWidth = gridWidth;
        this.gridHeight = gridHeight;
        this.cellWidth = cellWidth;
        this.density = density;
        this.kinematicViscosity = kinematicViscosity;
        this.linearSolverConfig = linearSolverConfig;
        this.threadCount = threadCount;
        this.applyGravity = applyGravity;
    }
}
