import argparse

from abc import ABC, abstractmethod
from SimulationWrapperTypes import *


class ValidationCase(ABC):
    _simulation: FluidSimulation
    _case_description: str
    _parser = None
    _args = None


    def __init__(self, case_description: str):
        self._case_description = case_description


    @abstractmethod
    def _set_up_arguments(self, default_grid_width: int, default_grid_height: int, default_density: float, default_kinematic_viscosity: float, default_time_step: float, default_iterations: int) -> None:
        self._parser = argparse.ArgumentParser(
            prog=type(self).__name__,
            description=self._case_description
        )

        self._parser.add_argument(
            "--grid-width",
            type=int,
            default=default_grid_width,
            help="Number of cells along the x axis"
        )

        self._parser.add_argument(
            "--grid-height",
            type=int,
            default=default_grid_height,
            help="Number of cells along the y axis"
        )

        self._parser.add_argument(
            "--density",
            type=float,
            default=default_density,
            help="Density of the fluid")
    
        self._parser.add_argument(
            "--kinematic-viscosity",
            type=float,
            default=default_kinematic_viscosity,
            help="Kinematic viscosity of the fluid"
        )
    
        self._parser.add_argument(
            "--time-step",
            type=float,
            default=default_time_step,
            help="Elapsed time between simulation steps"
        )
    
        self._parser.add_argument(
            "--iterations",
            type=int,
            default=default_iterations,
            help="Number of steps the simulation will take before analize the results"
        )


    @abstractmethod
    def _initialize(self) -> None:
        if self._parser is None:
            return
        
        self._args = self._parser.parse_args()

        print("Setting up...")
        width: c_int = self._args.grid_width
        height: c_int = self._args.grid_height
        cell_width: c_float = 1.0 / width
        density: c_float = self._args.density
        kinematic_viscosity: c_float = self._args.kinematic_viscosity
        linear_solver_iteration_count: c_int = 30
    
        self._simulation = FluidSimulation(width, height, cell_width, density, kinematic_viscosity, linear_solver_iteration_count)


    def _run_simulation(self) -> None:
        if self._simulation is None:
            return
        
        time_step: c_float = self._args.time_step
        simulation_iteration_count: int = self._args.iterations

        print("Running simulation...")
        progress = 0

        for k in range(simulation_iteration_count):
            self._simulation.step(time_step)

            progress = k / simulation_iteration_count * 100
            print(f"\rProgress: {progress:.2f}%", end="", flush=True)
        print("\rProgress: 100.0%")

    @abstractmethod
    def _analize_results(self) -> None:
        if self._simulation is None:
            return


    def run(self) -> None:
        self._set_up_arguments()
        self._initialize()
        self._run_simulation()
        self._analize_results()
