import matplotlib.pyplot as plt

from SimulationWrapperTypes import *
from ValidationCase import ValidationCase


class LidDrivenCavity(ValidationCase):
    def _set_up_arguments(self) -> None:
        super()._set_up_arguments(
            default_grid_width=33,
            default_grid_height=33,
            default_density=1.0,
            default_kinematic_viscosity=1e-05,
            default_time_step=1.0 / 120.0,
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

        moving_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(lid_speed, 0.0)))
        static_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(0.0, 0.0)))

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

        for i in range(width):
            for j in range(height):
                vel: Vec2f = self._simulation.get_velocity(i, j)
                ax.quiver(1.0 / width * i, 1.0 / height * j, vel.x, vel.y)

        plt.show()


if __name__ == "__main__":
    case = LidDrivenCavity("Sets up and runs the fluid simulation in a sealed square shaped box with only the top lid moving at a constant speed")
    case.run()
