import os
import argparse

import sys
sys.path.append("FluidSolverCore/out/build/debug")
import FluidSolverPython as fs

from Analysis.ParameterSeries import ParameterSeries
from Analysis.Validation.CaseConfig import CaseConfig
from Analysis.Validation.ValidationCase import ValidationCase
from Analysis.Validation.LidDrivenCavity import LidDrivenCavity
from Analysis.Validation.PoiseuilleFlow import PoiseuilleFlow

from typing import List


parser = argparse.ArgumentParser(
    "Grid Scaling",
    "Runs a validation case with an increasing grid size and measures the time taken per iteration"
)

parser.add_argument(
    "--case",
    type=str,
    default=None,
    help="Validation case with which to perform the benchmark",
    required=True
)

args = parser.parse_args()

config = CaseConfig(
    grid_width=None,
    grid_height=None,
    density=1.0,
    kinematic_viscosity=1e-03,
    time_step=1.0/2000.0,
    solver_iteration_count=30,
    simulation_iteration_count=1000
)

case_class: ValidationCase
case_name = args.case
match case_name:
    case "lid-driven-cavity":
        case_class = LidDrivenCavity
    case "poiseuille-flow":
        case_class = PoiseuilleFlow
    case _:
        raise ValueError(f"The provided case: {case_name} was not recognized")


config = case_class.get_default_config()
for grid_size in ParameterSeries.arithmetic(10, 40, 20):
    print(f"\nRunning the case with grid size: {grid_size}x{grid_size}...")

    config.grid_width = grid_size
    config.grid_height = grid_size
    case = case_class(config)
    
    case.run(case.print_progress)

    id = "============= COMPLETE SIMULATION STEP ============="
    step_avg_duration = fs.Profiler.get_instance().get_task_average_duration(id).total_seconds() * 1000

    print(f"\tComplete step: {step_avg_duration:.2f}ms")

    fs.Profiler.get_instance().clear_data()
