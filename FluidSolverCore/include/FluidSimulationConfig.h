enum LinearSolverAlgorithm {
    JACOBI,
    // Not implemented
    GAUSS_SEIDEL,
    // Not implemented
    RED_BLACK_GAUSS_SEIDEL
};

struct LinearSolverConfig {
    LinearSolverAlgorithm algorithm = LinearSolverAlgorithm::JACOBI;
    unsigned int iterationCount = 30;
};

struct FluidSimulationConfig {
	int gridWidth;
	int gridHeight;
	float cellWidth;
	float density;
	float kinematicViscosity;
    LinearSolverConfig linearSolverConfig;
    unsigned int threadCount;

    FluidSimulationConfig(int gridWidth, int gridHeight, float cellWidth, float density = 1, float kinematicViscosity = 0,
                          LinearSolverConfig linearSolverConfig = {}, unsigned int threadCount = 0, bool useMarkerParticles = false) :
        gridWidth(gridWidth),
        gridHeight(gridHeight),
        cellWidth(cellWidth),
        density(density),
        kinematicViscosity(kinematicViscosity),
        linearSolverConfig(linearSolverConfig),
        threadCount(threadCount) { }
};

// For a future GPU implementation we should proabably have a separate FluidSimulationGPU class
// since some settings here don't make sense in the GPU
