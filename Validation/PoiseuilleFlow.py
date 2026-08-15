import matplotlib.pyplot as plt

from SimulationWrapperTypes import *
from ValidationCase import ValidationCase


class PoiseuilleFlow(ValidationCase):
    def _set_up_arguments(self) -> None:
        super()._set_up_arguments(
            default_grid_width=33,
            default_grid_height=33,
            default_density=1.0,
            default_kinematic_viscosity=1e-05,
            default_time_step=1.0 / 120.0,
            default_iterations=500
        )

        self._parser.add_argument(
            "--inlet-speed",
            type=float,
            default=10.0,
            help="Speed at which the fluid will enter from the left into the pipe"
        )


    def _initialize(self) -> None:
        super()._initialize()

        inlet_speed: float = self._args.inlet_speed

        static_wall = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(0.0, 0.0)))
        inlet = CellConfig(CellData(CellType.FLUID), BoundaryData(BoundaryType.DIRICHLET, Vec2f(inlet_speed, 0.0)))
        outlet = CellConfig(CellData(CellType.FLUID), BoundaryData(BoundaryType.HOMOGENEOUS_NEUMANN))

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

        for i in range(width):
            for j in range(height):
                vel: Vec2f = self._simulation.get_velocity(i, j)
                ax.quiver(1.0 / width * i, 1.0 / height * j, vel.x, vel.y)

        plt.show()


if __name__ == "__main__":
    case = PoiseuilleFlow("Sets up and runs the fluid simulation inside a pipe where fluid enters from the left and leaves from the right")
    case.run()
