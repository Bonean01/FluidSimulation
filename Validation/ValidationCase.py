import argparse
import os

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
    def _set_up_arguments(self) -> None:
        self._parser = argparse.ArgumentParser(
            prog=os.path.splitext(os.path.basename(__file__))[0],
            description=self._case_description
        )

        self._parser.add_argument(
            "--grid-width",
            type=int,
            default=33,
            help="Number of cells along the x axis"
        )

        self._parser.add_argument(
            "--grid-height",
            type=int,
            default=33,
            help="Number of cells along the y axis"
        )

        self._parser.add_argument(
            "--density",
            type=float,
            default=1.0,
            help="Density of the fluid")
    
        self._parser.add_argument(
            "--kinematic-viscosity",
            type=float,
            default=1e-05,
            help="Kinematic viscosity of the fluid"
        )
    
        self._parser.add_argument(
            "--time-step",
            type=float,
            default=1.0 / 120.0,
            help="Elapsed time between simulation steps"
        )
    
        self._parser.add_argument(
            "--iterations",
            type=int,
            default=1000,
            help="Number of steps the simulation will take before analize the results"
        )


    @abstractmethod
    def _initialize(self) -> None:
        if self._parser is None:
            return
        
        print("Setting up...")
        self._args = self._parser.parse_args()

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
        for _ in range(simulation_iteration_count):
            self._simulation.step(time_step)


    @abstractmethod
    def _analize_results(self) -> None:
        if self._simulation is None:
            return


    def run(self) -> None:
        self._set_up_arguments()
        self._initialize()
        self._run_simulation()
        self._analize_results()
