import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from abc import ABC, abstractmethod
from Analysis.Validation.ValidationCase import ValidationCase


class Benchmark(ABC):
    case_class: ValidationCase
    case: ValidationCase


    def __init__(self, validation_case_name):
        self.case_class = ValidationCase.detect_validation_case_from_name(validation_case_name)


    @classmethod
    def set_up_arguments(cls, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            "--case",
            type=str,
            default=None,
            help="Validation case with which to perform the benchmark",
            required=True
        )


    @abstractmethod
    def run(self) -> None:
        pass


    @staticmethod
    def reset_profiler() -> float:
        """
        Resets the simulation's profiler and returns the number of milliseconds
        that a single step took to execute on average since the last reset
        """
        id = "============= COMPLETE SIMULATION STEP ============="
        profiler = fs.Profiler.get_instance()
        step_avg_duration = profiler.get_task_average_duration(id)
        step_avg_duration_ms = step_avg_duration.total_seconds() * 1000

        fs.Profiler.get_instance().clear_data()
        return step_avg_duration_ms
