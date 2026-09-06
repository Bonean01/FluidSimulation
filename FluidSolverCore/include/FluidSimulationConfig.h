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
	float density = 1;
	float kinematicViscosity = 0;
    unsigned int threadCount = 1;
    LinearSolverConfig linearSolverConfig{};
};

// For a future GPU implementation we should proabably have a separate FluidSimulationGPU class
// since settings like LinearSolverAlgorithm::JACOBI or threadCount don't make sense in the GPU
