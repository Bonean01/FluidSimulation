import os
import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")
from FluidSolverPython import *

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
        case = LidDrivenCavity()
    case "poiseuille-flow":
        case = PoiseuilleFlow()
    case _:
        raise ValueError(f"The provided case: {case_name} was not recognized")



def get_thread_count_list(max_threads: int, increment: int = 1) -> List[int]:
    res = []

    for count in range(1, max_threads, increment):
        res.append(count)

    res.append(max_threads)

    return res


max_threads = os.cpu_count()
thread_count_list = get_thread_count_list(max_threads, increment=5)


for thread_count in thread_count_list:
    set_num_threads(thread_count)
    print(f"\nRunning the case with {thread_count} threads...")
    print("==================================================")
    case.run()


    id = "============= COMPLETE SIMULATION STEP ============="
    step_avg_duration = Profiler.get_instance().get_task_average_duration(id)

    print(f"{id}: {step_avg_duration:.2f}ms")

    Profiler.get_instance().clear_data()
