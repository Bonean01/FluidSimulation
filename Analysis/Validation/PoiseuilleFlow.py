import matplotlib.pyplot as plt
import numpy as np
import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from typing import override

from .ValidationCase import ValidationCase
from .CaseConfig import CaseConfig



class PoiseuilleFlowConfig(CaseConfig):
    inlet_speed: float

    def __init__(self, grid_width=66, grid_height=33, density=1.0, kinematic_viscosity=1e-05, time_step=1.0/2000.0, solver_iteration_count=30, simulation_iteration_count=2000, inlet_speed=30.0):
        super().__init__(grid_width, grid_height, density, kinematic_viscosity, time_step, solver_iteration_count, simulation_iteration_count)
        self.inlet_speed = inlet_speed




class PoiseuilleFlow(ValidationCase):
    def __init__(self, config: PoiseuilleFlowConfig):
        self._config = config


    @override
    def _initialize(self):
        super()._initialize()

        static_wall = fs.CellConfig(fs.CellData(fs.CellType.SOLID), fs.BoundaryData(fs.BoundaryCondition.DIRICHLET, prescribed_velocity=fs.Vec2f(0.0, 0.0)))
        inlet = fs.CellConfig(fs.CellData(fs.CellType.FLUID), fs.BoundaryData(fs.BoundaryCondition.DIRICHLET, prescribed_velocity=fs.Vec2f(self._config.inlet_speed, 0.0)))
        outlet = fs.CellConfig(fs.CellData(fs.CellType.FLUID), fs.BoundaryData(fs.BoundaryCondition.HOMOGENEOUS_NEUMANN))

        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        for i in range(width):
            self._simulation.set_cell(i, height - 1, static_wall)
            self._simulation.set_cell(i, 0, static_wall)

        for j in range(2, height - 2):
            self._simulation.set_cell(0, j, inlet)
            self._simulation.set_cell(width - 1, j, outlet)


    @override
    @classmethod
    def set_up_arguments(cls, parser):
        config = PoiseuilleFlowConfig()
        super().set_up_arguments(parser, config)
        parser.add_argument(
            "--inlet-speed",
            type=float,
            default=30.0,
            help="Speed at which the fluid will enter from the left into the pipe"
        )


    @override
    def plot_results(self):
        super().plot_results()

        print("Drawing results...")
        _, ax = plt.subplots()
        
        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        speed_grid = np.zeros((width, height))

        for i in range(width):
            for j in range(height):
                vel = self._simulation.get_velocity(i, j)
                speed_grid[i, j] = np.sqrt(vel.x**2 + vel.y**2)


        X = 2 * np.arange(width) / width
        Y = np.arange(height) / height

        X, Y = np.meshgrid(X, Y, indexing="ij")

        ax.contour(X, Y, speed_grid, levels=10, colors="black", alpha=0.3)
        im = ax.imshow(speed_grid.T, origin="lower", cmap="viridis", extent=[0, 2, 0, 1])

        plt.colorbar(im, label="Speed")

        plt.show()


    @staticmethod
    def run_cli() -> None:
        parser = argparse.ArgumentParser(
            prog="Poiseuille Flow",
            description="Sets up and runs the fluid simulation inside a pipe where fluid enters from the left and leaves from the right"
        )

        PoiseuilleFlow.set_up_arguments(parser)
        args = parser.parse_args()

        case = PoiseuilleFlow(
            PoiseuilleFlowConfig(
                args.grid_width,
                args.grid_height,
                args.density,
                args.kinematic_viscosity,
                args.time_step,
                args.solver_iteration_count,
                args.simulation_iteration_count,
                args.inlet_speed
            )
        )

        case.print_parameters(args)
        case.run(progress_callback=case.print_progress)
        print()
        case.plot_results()


if __name__ == "__main__":
    PoiseuilleFlow.run_cli()
