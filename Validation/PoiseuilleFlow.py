import numpy as np
import matplotlib.pyplot as plt

from SimulationWrapperTypes import *

grid_width: int = 66
grid_height: int = 33
pipe_diameter: float = 1.0
cell_width: float = pipe_diameter / grid_height
density: float = 1.0
kinematic_viscosity: float = 1e-05
iteration_count: int = 30

time_step = 1.0 / 120.0
simulation_iteration_count = 100

print("Setting up...")
simulation: FluidSimulation = FluidSimulation(grid_width, grid_height, cell_width, density, kinematic_viscosity, iteration_count)

wall_data: CellData = CellData(CellType.SOLID, BoundaryType.NO_SLIP, True, Vec2f(0.0, 0.0))
inlet_data: CellData = CellData(CellType.FLUID, BoundaryType.VELOCITY_INLET, True, Vec2f(1.0, 0.0))
outlet_data: CellData = CellData(CellType.FLUID, BoundaryType.VELOCITY_OUTLET, True, Vec2f(1.0, 0.0))

for i in range(grid_width):
    simulation.SetCellData(i, 0, wall_data)
    simulation.SetCellData(i, grid_height - 1, wall_data)

for j in range(1, grid_height - 1):
    simulation.SetCellData(0, j, inlet_data)
    simulation.SetCellData(grid_width - 1, j, outlet_data)


print("Running simulation...")
for k in range(simulation_iteration_count):
    simulation.Step(time_step)


print("Drawing results...")
X = np.arange(0.0, 1.0, 0.05)
Y = np.arange(0.0, 1.0, 0.05)
U, V = np.meshgrid(X, Y)

fig, ax = plt.subplots()

for i in range(grid_width):
    for j in range(grid_height):
        vel: Vec2f = simulation.GetVelocity(i, j)
        ax.quiver(3.0 / grid_width * i, pipe_diameter / grid_height * j, vel.x, vel.y)

plt.show()

