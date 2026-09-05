import os
import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from Analysis.ParameterSeries import ParameterSeries

from Analysis.Validation.CaseConfig import CaseConfig
from Analysis.Validation.ValidationCase import ValidationCase
from .Benchmark import Benchmark
from typing import override


class ThreadScaling(Benchmark):
    @staticmethod
    def print_thread_count(thread_count: int) -> None:
        print(f"\nRunning the case with {thread_count} threads...")


    @override
    def _get_iteration_config(self, thread_count) -> CaseConfig:
        fs.set_num_threads(thread_count)
        return self._case_class.get_default_config()


    def run_cli() -> None:
        parser = argparse.ArgumentParser(
            "Thread Scaling",
            "Runs a validation case with an increasing number of threads and measures the time taken per iteration"
        )

        ThreadScaling.set_up_arguments(parser)
        args = parser.parse_args()

        thread_scaling = ThreadScaling(args.case)
        thread_scaling.run(
            parameter_list=ParameterSeries.geometric(min=1, max=os.cpu_count(), range=2),
            current_parameter_callback=ThreadScaling.print_thread_count,
            step_duration_callback=Benchmark.print_step_duration,
            case_progress_callback=ValidationCase.print_progress
        )


if __name__ == "__main__":
    ThreadScaling.run_cli()
