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
cell_width = 0.01
density = 1
kinematic_viscosity = 1e-04
solver_iterations = 30
simulation_iterations = 100

time_step = 1.0 / 120.0

simulation = FluidSimulation(width, height, cell_width, density, kinematic_viscosity, solver_iterations)
wall_config = CellConfig(CellData(CellType.SOLID), BoundaryData(BoundaryCondition.DIRICHLET, Vec2f(0.0, 0.0)))

for i in range(width):
    simulation.set_cell(i, 0, wall_config)
    simulation.set_cell(i, height - 1, wall_config)

for j in range(height):
    simulation.set_cell(0, j, wall_config)
    simulation.set_cell(width - 1, j, wall_config)

simulation.step(time_step)

print(Profiler.get_instance().get_IDs())
