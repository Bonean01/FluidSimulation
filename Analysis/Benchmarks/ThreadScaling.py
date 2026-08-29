import os
import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from Analysis.ParameterSeries import ParameterSeries
from Analysis.Validation.ValidationCase import ValidationCase
from Analysis.Validation.LidDrivenCavity import LidDrivenCavity
from Analysis.Validation.PoiseuilleFlow import PoiseuilleFlow

from typing import List


parser = argparse.ArgumentParser(
    "Thread Scaling",
    "Runs a validation case with an increasing number of threads and measures the time taken per iteration"
)

parser.add_argument(
    "--case",
    type=str,
    default=None,
    help="Validation case with which to perform the benchmark",
    required=True
)

args = parser.parse_args()

case: ValidationCase
case_name = args.case
match case_name:
    case "lid-driven-cavity":
        case_class = LidDrivenCavity
    case "poiseuille-flow":
        case_class = PoiseuilleFlow
    case _:
        raise ValueError(f"The provided case: {case_name} was not recognized")


max_threads = os.cpu_count()

for thread_count in ParameterSeries.geometric(min=1, max=max_threads, range=2):
    fs.set_num_threads(thread_count)
    print(f"\nRunning the case with {thread_count} threads...")
    config = case.get_default_config()
    case = case_class(config)
    case.run(case.print_progress)


    id = "============= COMPLETE SIMULATION STEP ============="
    step_avg_duration = fs.Profiler.get_instance().get_task_average_duration(id).total_seconds() * 1000

    print(f"\tComplete step: {step_avg_duration:.2f}ms")

    fs.Profiler.get_instance().clear_data()
