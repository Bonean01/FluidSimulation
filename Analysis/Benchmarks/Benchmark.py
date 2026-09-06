import argparse
import sys

sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from abc import ABC, abstractmethod
from typing import Callable, Any, List
from Analysis.Validation.CaseConfig import CaseConfig
from Analysis.Validation.ValidationCase import ValidationCase
from Analysis.Utils.CaseRegistry import CaseRegistry


class Benchmark(ABC):
    _case_class: ValidationCase
    _case: ValidationCase


    def __init__(self, validation_case_name):
        self._case_class = CaseRegistry.detect_validation_case_from_name(validation_case_name)


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
    def _get_iteration_config(self, parameter) -> CaseConfig:
        pass

    
    def run(self, parameter_list: List[Any],
            current_parameter_callback: Callable[[Any], None] = None,
            step_duration_callback: Callable[[float], None] = None,
            case_progress_callback: Callable[[float], None] = None
    ) -> None:
        for parameter in parameter_list:
            if current_parameter_callback is not None:
                current_parameter_callback(parameter)

            config = self._get_iteration_config(parameter)
            self._case = self._case_class(config)
            self._case.run(case_progress_callback)
            step_avg_duration_ms = Benchmark.reset_profiler()

            if step_duration_callback is not None:
                step_duration_callback(step_avg_duration_ms)


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


    @staticmethod
    def print_step_duration(step_avg_duration_ms):
        print(f"\tComplete step: {step_avg_duration_ms:.2f}ms")
