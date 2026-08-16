import matplotlib.pyplot as plt
import numpy as np

from SimulationWrapperTypes import *
from ValidationCase import ValidationCase


class LidDrivenCavity(ValidationCase):
    def _set_up_arguments(self) -> None:
        super()._set_up_arguments(
            default_grid_width=33,
            default_grid_height=33,
            default_density=1.0,
            default_kinematic_viscosity=1e-03,
            default_time_step=1.0 / 1000.0,
            default_iterations=1000
        )

        self._parser.add_argument(
            "--lid-speed",
            type=float,
            default=1.0,
            help="Speed at which the lid will move sideways (< 0 => left, > 0 => right)"
        )


    def _initialize(self) -> None:
        super()._initialize()

        lid_speed: float = self._args.lid_speed

        moving_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, prescribed_velocity=Vec2f(lid_speed, 0.0)))
        static_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, prescribed_velocity=Vec2f(0.0, 0.0)))

        width = self._simulation.get_grid_width()
        height = self._simulation.get_grid_height()

        for i in range(width):
            self._simulation.set_cell(i, height - 1, moving_wall)
            self._simulation.set_cell(i, 0, static_wall)

        for j in range(height - 1):
            self._simulation.set_cell(0, j, static_wall)
            self._simulation.set_cell(width - 1, j, static_wall)


    def _analize_results(self) -> None:
        super()._analize_results()

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

        ax.imshow(
            speed_grid.T,
            origin="lower",
            interpolation="none",
            extent=[0, 1, 0, 1],
        )

        ax.quiver(
            X,
            Y,
            u_grid,
            v_grid,
        )

        plt.show()



if __name__ == "__main__":
    case = LidDrivenCavity("Sets up and runs the fluid simulation in a sealed square shaped box with only the top lid moving at a constant speed")
    case.run()
