import matplotlib.pyplot as plt
import numpy as np

from ..SimulationWrapperTypes import *
from .ValidationCase import ValidationCase


class PoiseuilleFlow(ValidationCase):
    def _set_up_arguments(self) -> None:
        super()._set_up_arguments(
            default_grid_width=66,
            default_grid_height=33,
            default_density=1.0,
            default_kinematic_viscosity=1e-05,
            default_time_step=1.0 / 2000.0,
            default_iterations=2000
        )

        self._parser.add_argument(
            "--inlet-speed",
            type=float,
            default=30.0,
            help="Speed at which the fluid will enter from the left into the pipe"
        )


    def _initialize(self) -> None:
        super()._initialize()

        inlet_speed: float = self._args.inlet_speed

        static_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, prescribed_velocity=Vec2f(0.0, 0.0)))
        inlet = CellConfig(CellData(CellType.FLUID), BoundaryData(BoundaryCondition.DIRICHLET, prescribed_velocity=Vec2f(inlet_speed, 0.0)))
        outlet = CellConfig(CellData(CellType.FLUID), BoundaryData(BoundaryCondition.HOMOGENEOUS_NEUMANN))

        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        for i in range(width):
            self._simulation.set_cell(i, height - 1, static_wall)
            self._simulation.set_cell(i, 0, static_wall)

        for j in range(2, height - 2):
            self._simulation.set_cell(0, j, inlet)
            self._simulation.set_cell(width - 1, j, outlet)


    def _analize_results(self) -> None:
        super()._analize_results()

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


if __name__ == "__main__":
    case = PoiseuilleFlow("Sets up and runs the fluid simulation inside a pipe where fluid enters from the left and leaves from the right")
    case.run()
