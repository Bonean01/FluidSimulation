import os
import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from Analysis.ParameterSeries import ParameterSeries

from .Benchmark import Benchmark
from typing import override


class ThreadScaling(Benchmark):
    @override
    def run(self) -> None:
        super().run()
        max_threads = os.cpu_count()
        config = self.case_class.get_default_config()
        
        for thread_count in ParameterSeries.geometric(min=1, max=max_threads, range=2):
            fs.set_num_threads(thread_count)
            print(f"\nRunning the case with {thread_count} threads...")
            
            self.case = self.case_class(config)
            self.case.run(self.case.print_progress)

            step_avg_duration_ms = Benchmark.reset_profiler()
            print(f"\tComplete step: {step_avg_duration_ms:.2f}ms")


    def run_cli() -> None:
        parser = argparse.ArgumentParser(
            "Thread Scaling",
            "Runs a validation case with an increasing number of threads and measures the time taken per iteration"
        )

        ThreadScaling.set_up_arguments(parser)
        args = parser.parse_args()

        thread_scaling = ThreadScaling(args.case)
        thread_scaling.run()


if __name__ == "__main__":
    ThreadScaling.run_cli()
