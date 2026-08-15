import matplotlib.pyplot as plt
import sys
import argparse

from SimulationWrapperTypes import *


def main():
    parser = argparse.ArgumentParser(
        prog="LidDrivenCavity",
        description="Sets up and runs the fluid simulation in a sealed square shaped box with only the top lid moving at a constant speed"
    )

    parser.add_argument(
        "--grid-width",
        type=int,
        default=33,
        help="Number of cells along the x axis"
    )

    parser.add_argument(
        "--grid-height",
        type=int,
        default=33,
        help="Number of cells along the y axis"
    )

    parser.add_argument(
        "--density",
        type=float,
        default=1.0,
        help="Density of the fluid")

    parser.add_argument(
        "--kinematic-viscosity",
        type=float,
        default=1e-05,
        help="Kinematic viscosity of the fluid"
    )

    parser.add_argument(
        "--lid-speed",
        type=float,
        default=1.0,
        help="Speed at which the lid will move sideways (< 0 => left, > 0 => right)"
    )

    parser.add_argument(
        "--time-step",
        type=float,
        default=1.0 / 120.0,
        help="Elapsed time between simulation steps"
    )

    parser.add_argument(
        "--iterations",
        type=int,
        default=1000,
        help="Number of steps the simulation will take before drawing the result"
    )

    args = parser.parse_args()

    width: c_int = args.grid_width
    height: c_int = args.grid_height
    cell_width: c_float = 1.0 / width
    density: c_float = args.density
    kinematic_viscosity: c_float = args.kinematic_viscosity
    linear_solver_iteration_count: c_int = 30

    lid_speed: float = args.lid_speed
    time_step: c_float = args.time_step
    simulation_iteration_count: int = args.iterations


    print(
        "\nRunning lid-driven cavity with parameters:\n" +
        "================================================\n" +
        f"grid_width: {width}\n" +
        f"grid_height: {height}\n" +
        f"density: {density}\n" +
        f"kinematic_viscosity: {kinematic_viscosity}\n" +
        f"lid_speed: {lid_speed}\n" +
        f"time_step: {time_step}\n" +
        f"iterations: {simulation_iteration_count}\n" +
        "================================================\n"
    )


    print("Setting up...")
    simulation = FluidSimulation(width, height, cell_width, density, kinematic_viscosity, linear_solver_iteration_count)

    moving_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(lid_speed, 0.0)))
    static_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(0.0, 0.0)))

    for i in range(width):
        simulation.set_cell(i, height - 1, moving_wall)
        simulation.set_cell(i, 0, static_wall)

    for j in range(height - 1):
        simulation.set_cell(0, j, static_wall)
        simulation.set_cell(width - 1, j, static_wall)


    print("Running simulation...")
    for _ in range(simulation_iteration_count):
        simulation.step(time_step)


    print("Drawing results...")
    _, ax = plt.subplots()

    for i in range(width):
        for j in range(height):
            vel: Vec2f = simulation.GetVelocity(i, j)
            ax.quiver(1.0 / width * i, 1.0 / height * j, vel.x, vel.y)

    plt.show()



if __name__ == "__main__":
    main()
