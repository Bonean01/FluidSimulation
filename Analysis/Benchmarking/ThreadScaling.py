import os

import sys
sys.path.append("FluidSolverCore/out/build/debug")
from FluidSolverPython import *


max_threads = os.cpu_count()

thread_count_list = []

n = 1
while n < max_threads:
    thread_count_list.append(n)
    n *= 2
thread_count_list.append(max_threads)

width = 250
height = 250
cell_width = 1.0 / width
density = 1
kinematic_viscosity = 1e-03
solver_iterations = 30
simulation_iterations = 100

time_step = 1.0 / 120.0


for thread_count in range(1, max_threads + 1):
    #print(f"\nRunning the simulation with {thread_count} threads...")
    #print("========================================================")

    simulation = FluidSimulation(width, height, cell_width, density, kinematic_viscosity, solver_iterations)
    wall_config = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, Vec2f(0.0, 0.0)))

    for i in range(width):
        simulation.set_cell(i, 0, wall_config)
        simulation.set_cell(i, height - 1, wall_config)

    for j in range(height):
        simulation.set_cell(0, j, wall_config)
        simulation.set_cell(width - 1, j, wall_config)

    set_num_threads(thread_count)
    for _ in range(simulation_iterations):
        simulation.step(time_step)

    #profiler_results = Profiler.get_instance().get_results()
    #task_results = profiler_results.task_results

    #for task in task_results:
    #    duration_millis = task.average_duration.total_seconds() * 1000
    #    print(f"{task.id}: {duration_millis:.2f}ms")
    duration_millis = Profiler.get_instance().get_task_average_duration("============= COMPLETE SIMULATION STEP =============").total_seconds() * 1000
    print(f"({thread_count}, {duration_millis:.2f})")
    Profiler.get_instance().reset()
