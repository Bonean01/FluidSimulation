class CaseConfig:
    grid_width: int
    grid_height: int
    density: float
    kinematic_viscosity: float
    time_step: float
    solver_iteration_count: int
    simulation_iteration_count: int


    def __init__(self, grid_width, grid_height, density, kinematic_viscosity, time_step, solver_iteration_count, simulation_iteration_count):
        self.grid_width = grid_width
        self.grid_height = grid_height
        self.density = density
        self.kinematic_viscosity = kinematic_viscosity
        self.time_step = time_step
        self.solver_iteration_count = solver_iteration_count
        self.simulation_iteration_count = simulation_iteration_count