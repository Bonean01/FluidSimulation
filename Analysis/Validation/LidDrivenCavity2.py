import matplotlib.pyplot as plt
import numpy as np
import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
from FluidSolverPython import *

from .ValidationCase2 import ValidationCase


class LidDrivenCavity(ValidationCase):
    _lid_speed: float


    def __init__(self, grid_width: int, grid_height: int, density: float, kinematic_viscosity: float, time_step: float, solver_iteration_count: int, simulation_iteration_count: int, lid_speed: float):
            super().__init__(grid_width, grid_height, density, kinematic_viscosity, time_step, solver_iteration_count, simulation_iteration_count)
            self._lid_speed = lid_speed

    
    def _set_up_arguments(self, parser) -> None:
        super()._set_up_arguments(parser)

        parser.add_argument(
            "--lid-speed",
            type=float,
            default=1.0,
            help="Speed at which the lid will move sideways (< 0 => left, > 0 => right)"
        )


    def _initialize(self) -> None:
        super()._initialize()

        moving_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, prescribed_velocity=Vec2f(self._lid_speed, 0.0)))
        static_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, prescribed_velocity=Vec2f(0.0, 0.0)))

        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        for i in range(width):
            self._simulation.set_cell(i, height - 1, moving_wall)
            self._simulation.set_cell(i, 0, static_wall)

        for j in range(height - 1):
            self._simulation.set_cell(0, j, static_wall)
            self._simulation.set_cell(width - 1, j, static_wall)


    def plot_results(self) -> None:
        super()._plot_results()

        print("Drawing results...")
        _, ax = plt.subplots()

        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        speed_grid = np.zeros((width, height))
        u_grid = np.zeros((width, height))
        v_grid = np.zeros((width, height))

        for i in range(width):
            for j in range(height):
                vel = self._simulation.get_velocity(i, j)

                u_grid[i, j] = vel.x
                v_grid[i, j] = vel.y
                speed_grid[i, j] = np.sqrt(vel.x**2 + vel.y**2)

        X = np.arange(width) / width
        Y = np.arange(height) / height

        X, Y = np.meshgrid(X, Y, indexing="ij")

        im = ax.imshow(
            speed_grid.T,
            origin="lower",
            interpolation="none",
            extent=[0, 1, 0, 1],
        )

        ax.streamplot(X.T, Y.T, u_grid.T, v_grid.T, color=(0.0, 0.0, 0.0, 0.4))

        plt.colorbar(im, label="Speed")

        plt.show()



if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="Lid Driven Cavity",
        description="Sets up and runs the fluid simulation in a sealed square shaped box with only the top lid moving at a constant speed"
    )
    LidDrivenCavity.set_up_arguments(parser)
    args = parser.parse_args()
    case = LidDrivenCavity(
        args.grid_width,
        args.grid_height,
        args.density,
        args.kinematic_viscosity,
        args.time_step,
        args.solver_iteration_count,
        args.simulation_iteration_count,
        args.lid_speed
    )
    case.print_parameters(args)
    case.run()
    case.plot_results()
