import matplotlib.pyplot as plt

from SimulationWrapperTypes import *


def main():
    grid_width: int = 67
    grid_height: int = 33
    pipe_diameter: float = 1.0
    inlet_speed: float = 30.0
    cell_width: float = pipe_diameter / grid_height
    density: float = 1.0
    kinematic_viscosity: float = 1e-05
    iteration_count: int = 30

    time_step = 1.0 / 120.0
    simulation_iteration_count = 100

    print("Setting up...")
    simulation = FluidSimulation(grid_width, grid_height, cell_width, density, kinematic_viscosity, iteration_count)


    wall_cell = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(0.0, 0.0)))
    inlet_cell = CellConfig(CellData(CellType.FLUID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(inlet_speed, 0.0)))
    outlet_cell = CellConfig(CellData(CellType.FLUID), BoundaryData(BoundaryType.HOMOGENEOUS_NEUMANN))


    for i in range(grid_width):
        simulation.set_cell(i, 0, wall_cell)
        simulation.set_cell(i, grid_height - 1, wall_cell)

    for j in range(2, grid_height - 2):
        simulation.set_cell(0, j, inlet_cell)
        simulation.set_cell(grid_width - 1, j, outlet_cell)


    print("Running simulation...")
    for _ in range(simulation_iteration_count):
        simulation.step(time_step)


    print("Drawing results...")
    _, ax = plt.subplots()

    for i in range(grid_width):
        for j in range(grid_height):
            vel: Vec2f = simulation.GetVelocity(i, j)
            ax.quiver(3.0 / grid_width * i, pipe_diameter / grid_height * j, vel.x, vel.y)

    plt.show()


if __name__ == "__main__":
    main()
