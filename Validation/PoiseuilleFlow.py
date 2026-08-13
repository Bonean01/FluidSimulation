import numpy as np
import matplotlib.pyplot as plt

from SimulationWrapperTypes import *

grid_width: int = 67
grid_height: int = 33
pipe_diameter: float = 1.0
inlet_speed: float = 30.0
cell_width: float = pipe_diameter / grid_height
density: float = 1.0
kinematic_viscosity: float = 1e-05
iteration_count: int = 30

time_step = 1.0 / 120.0
simulation_iteration_count = 100

print("Setting up...")
simulation: FluidSimulation = FluidSimulation(grid_width, grid_height, cell_width, density, kinematic_viscosity, iteration_count)


wall_cell: CellProperties = CellProperties(CellData(CellType.SOLID), BoundaryData(BoundaryType.NO_SLIP, Vec2f(0.0, 0.0)))
inlet_cell: CellProperties = CellProperties(CellData(CellType.FLUID), BoundaryData(BoundaryType.VELOCITY_INLET, Vec2f(inlet_speed, 0.0)))
outlet_cell: CellProperties = CellProperties(CellData(CellType.FLUID), BoundaryData(BoundaryType.VELOCITY_OUTLET))


for i in range(grid_width):
    simulation.SetCell(i, 0, wall_cell)
    simulation.SetCell(i, grid_height - 1, wall_cell)

for j in range(2, grid_height - 2):
    simulation.SetCell(0, j, inlet_cell)
    simulation.SetCell(grid_width - 1, j, outlet_cell)


print("Running simulation...")
for k in range(simulation_iteration_count):
    simulation.Step(time_step)


print("Drawing results...")
fig, ax = plt.subplots()

for i in range(grid_width):
    for j in range(grid_height):
        vel: Vec2f = simulation.GetVelocity(i, j)
        ax.quiver(3.0 / grid_width * i, pipe_diameter / grid_height * j, vel.x, vel.y)

plt.show()
