import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")

from Analysis.Utils.ParameterSeries import ParameterSeries

from Analysis.Validation.CaseConfig import CaseConfig
from Analysis.Validation.ValidationCase import ValidationCase
from .Benchmark import Benchmark
from typing import override


class GridScaling(Benchmark):
    @override
    def _get_iteration_config(self, grid_size) -> CaseConfig:
        config = self._case_class.get_default_config()
        config.grid_width = grid_size
        config.grid_height = grid_size

        return config


    @staticmethod
    def print_grid_size(grid_size: int) -> None:
        print(f"\nRunning the case with grid size: {grid_size}x{grid_size}...")


    def run_cli() -> None:
        parser = argparse.ArgumentParser(
            "Grid Scaling",
            "Runs a validation case with an increasing grid size and measures the time taken per iteration"
        )

        GridScaling.set_up_arguments(parser)
        args = parser.parse_args()

        grid_scaling = GridScaling(args.case)
        grid_scaling.run(
                    parameter_list=ParameterSeries.arithmetic(min=10, max=40, increment=20),
                    current_parameter_callback=GridScaling.print_grid_size,
                    step_duration_callback=Benchmark.print_step_duration,
                    case_progress_callback=ValidationCase.print_progress
                )


if __name__ == "__main__":
    GridScaling.run_cli()
