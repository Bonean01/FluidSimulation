import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from abc import ABC, abstractmethod
from typing import Callable

from .CaseConfig import CaseConfig



class ValidationCase(ABC):
    _simulation: fs.FluidSimulation
    # It would maybe be nice separating simulation config (width, height, cell_width, etc.)
    # from execution config (thread count) and parameter sweeps (what parameters to sweep and how)
    _config: CaseConfig


    def __init__(self, config: CaseConfig):
        self._config = config


    @staticmethod
    @abstractmethod
    def get_default_config() -> CaseConfig:
        pass


    def _initialize(self) -> None:
        cell_width = 1.0 / self._config.grid_width
        self._simulation = fs.FluidSimulation(self._config.grid_width, self._config.grid_height, cell_width, self._config.density, self._config.kinematic_viscosity, self._config.solver_iteration_count)


    def _run_simulation(self, progress_callback: Callable[[float], None] = None) -> None:
        progress = 0
        for k in range(self._config.simulation_iteration_count):
            self._simulation.step(self._config.time_step)
            
            progress = (k + 1) / self._config.simulation_iteration_count * 100
            if progress_callback is not None:
                progress_callback(progress)


    @classmethod
    def set_up_arguments(cls, parser: argparse.ArgumentParser, default_config: CaseConfig):
        parser.add_argument(
            "--grid-width",
            type=int,
            default=default_config.grid_width,
            help="Number of cells along the x axis"
        )

        parser.add_argument(
            "--grid-height",
            type=int,
            default=default_config.grid_height,
            help="Number of cells along the y axis"
        )

        parser.add_argument(
            "--density",
            type=float,
            default=default_config.density,
            help="Density of the fluid")
    
        parser.add_argument(
            "--kinematic-viscosity",
            type=float,
            default=default_config.kinematic_viscosity,
            help="Kinematic viscosity of the fluid"
        )
    
        parser.add_argument(
            "--time-step",
            type=float,
            default=default_config.time_step,
            help="Elapsed time between simulation steps"
        )

        parser.add_argument(
            "--solver-iteration-count",
            type=int,
            default=default_config.solver_iteration_count,
            help="Number of iterations the linear solvers will perform"
        )
    
        parser.add_argument(
            "--simulation-iteration-count",
            type=int,
            default=default_config.simulation_iteration_count,
            help="Number of steps the simulation will take before analize the results"
        )


    def print_parameters(self, args) -> None:
            print("\nRunning " + type(self).__name__ + " with parameters: ")
            print("=====================================================")
            for name, value in vars(args).items():
                print(f"{name}: {value}")
            print("=====================================================")


    def print_progress(self, progress: float) -> None:
        print(f"\rProgress: {progress:.2f}%", end="", flush=True)


    def plot_results(self) -> None:
        if self._simulation is None:
            return


    def run(self, progress_callback: Callable[[float], None] = None) -> None:
        self._initialize()
        self._run_simulation(progress_callback)




    # Potentially move this to a case registry class of sorts
    @staticmethod
    def detect_validation_case_from_name(name: str):
        """
        Returns the ValidationCase child class type given its name in a kebab-case string
        """
        from .LidDrivenCavity import LidDrivenCavity
        from .PoiseuilleFlow import PoiseuilleFlow
        
        case_class: ValidationCase
        match name:
            case "lid-driven-cavity":
                case_class = LidDrivenCavity
            case "poiseuille-flow":
                case_class = PoiseuilleFlow
            case _:
                raise ValueError(f"The provided case: {case_name} was not recognized")
            
        return case_class
