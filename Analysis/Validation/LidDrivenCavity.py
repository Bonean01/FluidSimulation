import matplotlib.pyplot as plt
import numpy as np
import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from typing import override

from .ValidationCase import ValidationCase
from .CaseConfig import CaseConfig



class LidDrivenCavityConfig(CaseConfig):
    lid_speed: float

    def __init__(self, grid_width=33, grid_height=33, density=1.0, kinematic_viscosity=1e-02, time_step=1.0/2000.0, solver_iteration_count=30, simulation_iteration_count=2000, lid_speed=1.0):
        super().__init__(grid_width, grid_height, density, kinematic_viscosity, time_step, solver_iteration_count, simulation_iteration_count)
        self.lid_speed = lid_speed





class LidDrivenCavity(ValidationCase):
    def __init__(self, config: LidDrivenCavityConfig = LidDrivenCavityConfig()):
        self._config = config


    @override
    @staticmethod
    def get_default_config() -> CaseConfig:
        super().get_default_config()
        return LidDrivenCavityConfig()


    @override
    def _initialize(self) -> None:
        super()._initialize()

        moving_wall = fs.CellConfig(fs.CellData(fs.CellType.SOLID), fs.BoundaryData(fs.BoundaryCondition.DIRICHLET, prescribed_velocity=fs.Vec2f(self._config.lid_speed, 0.0)))
        static_wall = fs.CellConfig(fs.CellData(fs.CellType.SOLID), fs.BoundaryData(fs.BoundaryCondition.DIRICHLET, prescribed_velocity=fs.Vec2f(0.0, 0.0)))

        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        for i in range(width):
            self._simulation.set_cell(i, height - 1, moving_wall)
            self._simulation.set_cell(i, 0, static_wall)

        for j in range(height - 1):
            self._simulation.set_cell(0, j, static_wall)
            self._simulation.set_cell(width - 1, j, static_wall)


    @override
    @classmethod
    def set_up_arguments(cls, parser: argparse.ArgumentParser) -> None:
        config = LidDrivenCavityConfig()
        super().set_up_arguments(parser, config)
        parser.add_argument(
            "--lid-speed",
            type=float,
            default=config.lid_speed,
            help="Speed at which the lid will move sideways (< 0 => left, > 0 => right)"
        )


    @override
    def plot_results(self) -> None:
        super().plot_results()

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


    @staticmethod
    def run_cli() -> None:
        parser = argparse.ArgumentParser(
            prog="Lid Driven Cavity",
            description="Sets up and runs the fluid simulation in a sealed square shaped box with only the top lid moving at a constant speed"
        )

        LidDrivenCavity.set_up_arguments(parser)
        args = parser.parse_args()

        config = LidDrivenCavityConfig(
            args.grid_width,
            args.grid_height,
            args.density,
            args.kinematic_viscosity,
            args.time_step,
            args.solver_iteration_count,
            args.simulation_iteration_count,
            args.lid_speed
        )
        case = LidDrivenCavity(config)

        case.print_parameters(args)
        case.run(progress_callback=case.print_progress)
        print()
        case.plot_results()


if __name__ == "__main__":
    LidDrivenCavity.run_cli()
