import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")

from Analysis.ParameterSeries import ParameterSeries

from .Benchmark import Benchmark
from typing import override


class GridScaling(Benchmark):
    @override
    def run(self) -> None:
        for grid_size in ParameterSeries.arithmetic(min=10, max=40, increment=20):
            print(f"\nRunning the case with grid size: {grid_size}x{grid_size}...")

            config = self.case_class.get_default_config()
            config.grid_width = grid_size
            config.grid_height = grid_size

            self.case = self.case_class(config)
            self.case.run(self.case.print_progress)

            step_avg_duration_ms = Benchmark.reset_profiler()
            print(f"\tComplete step: {step_avg_duration_ms:.2f}ms")


    def run_cli() -> None:
        parser = argparse.ArgumentParser(
            "Grid Scaling",
            "Runs a validation case with an increasing grid size and measures the time taken per iteration"
        )

        GridScaling.set_up_arguments(parser)
        args = parser.parse_args()

        grid_scaling = GridScaling(args.case)
        grid_scaling.run()


if __name__ == "__main__":
    GridScaling.run_cli()
