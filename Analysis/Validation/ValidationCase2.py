import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from abc import ABC, abstractmethod


class ValidationCase(ABC):
    _simulation: fs.FluidSimulation

    _grid_width: int
    _grid_height: int
    _density: float
    _kinematic_viscosity: float
    _time_step: float
    _time_step: float
    _solver_iteration_count: int
    _simulation_iteration_count: int


    def __init__(self, grid_width: int, grid_height: int, density: float, kinematic_viscosity: float, time_step: float, solver_iteration_count: int, simulation_iteration_count: int):
        self._grid_width = grid_width
        self._grid_height = grid_height
        self._density = density
        self._kinematic_viscosity = kinematic_viscosity
        self._time_step = time_step
        self._solver_iteration_count = solver_iteration_count
        self._simulation_iteration_count = simulation_iteration_count
    

    @staticmethod
    def set_up_arguments(self, parser) -> None:
        parser.add_argument(
            "--grid-width",
            type=int,
            default=self._grid_width,
            help="Number of cells along the x axis"
        )

        parser.add_argument(
            "--grid-height",
            type=int,
            default=self._grid_height,
            help="Number of cells along the y axis"
        )

        parser.add_argument(
            "--density",
            type=float,
            default=self._density,
            help="Density of the fluid")
    
        parser.add_argument(
            "--kinematic-viscosity",
            type=float,
            default=self._kinematic_viscosity,
            help="Kinematic viscosity of the fluid"
        )
    
        parser.add_argument(
            "--time-step",
            type=float,
            default=self._time_step,
            help="Elapsed time between simulation steps"
        )

        parser.add_argument(
            "--solver-iteration-count",
            type=int,
            default=self._solver_iteration_count,
            help="Number of iterations the linear solvers will perform"
        )
    
        parser.add_argument(
            "--simulation-iteration-count",
            type=int,
            default=self._simulation_iteration_count,
            help="Number of steps the simulation will take before analize the results"
        )


    def _initialize(self) -> None:
        cell_width = 1.0 / self._grid_width    
        self._simulation = fs.FluidSimulation(self._width, self._height, cell_width, self._density, self._kinematic_viscosity, self._solver_iteration_count)


    def print_parameters(self, args) -> None:
            if self._simulation is None:
                return
            
            print("\nRunning " + type(self).__name__ + " with parameters: ")
            print("=====================================================")
            print("cell-width: " + str(self._simulation.get_cell_width()))
            for name, value in vars(self._args).items():
                print(f"{name}: {value}")
            print("=====================================================")


    def _run_simulation(self) -> None:
        for _ in range(self._simulation_iteration_count):
            self._simulation.step(self._time_step)


    def plot_results(self) -> None:
        if self._simulation is None:
            return


    def run(self) -> None:
        self._initialize()
        self._run_simulation()
