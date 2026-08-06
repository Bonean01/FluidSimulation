import numpy as np
import matplotlib.pyplot as plt
import sys

from SimulationWrapperTypes import *


side_length: int = 33
cell_width: c_float = 1.0 / side_length
density: c_float = 1.0
kinematic_viscosity: c_float = 1e-05
iteration_count: c_int = 30

lid_speed: int = 1.0
time_step: c_float = 1.0 / 120.0
simulation_iteration_count: int = 1000


for i, string in enumerate(sys.argv):
    if i == 0 or not string.startswith("--"):
        continue

    match string:
        case "--grid-side-length":
            side_length = c_int(sys.argv[i + 1])
        case "--cell-width":
            cell_width = c_float(sys.argv[i + 1])
        case "--density":
            density = c_float(sys.argv[i + 1])
        case "--kinematic-viscosity":
            kinematic_viscosity = c_float(sys.argv[i + 1])
        case "--lid-speed":
            lid_speed = int(sys.argv[i + 1])
        case "--time-step":
            time_step = c_float(sys.argv[i + 1])
        case "--iterations":
            simulation_iteration_count = int(sys.argv[i + 1])
        case _:
            print(
                f"Unsupported option: {string}\n" +
                "Available options:\n" +
                "================================================\n" +
                "--grid-side-length\n" +
                "--cell-width\n" +
                "--density\n" +
                "--kinematic-viscosity\n" +
                "--lid-speed\n" +
                "--time-step\n" +
                "--iterations\n" + 
                "================================================\n"
            )
            quit()


print(
    "\nRunning lid-driven cavity with parameters:\n" +
    "================================================\n" +
    f"grid_side_length: {side_length}\n" +
    f"cell_width: {cell_width}\n"
    f"density: {density}\n" +
    f"kinematic_viscosity: {kinematic_viscosity}\n" +
    f"lid_speed: {lid_speed}\n" +
    f"time_step: {time_step}\n" +
    f"iterations: {simulation_iteration_count}\n" +
    "================================================\n"
)


print("Setting up...")
width: c_int = side_length
height: c_int = side_length
simulation: FluidSimulation = FluidSimulation(width, height, cell_width, density, kinematic_viscosity, iteration_count)

lid_data: CellData = CellData(CellType.SOLID, BoundaryType.NO_SLIP, Vec2f(lid_speed, 0.0))
wall_data: CellData = CellData(CellType.SOLID, BoundaryType.NO_SLIP, Vec2f(0.0, 0.0))

for i in range(width):
    simulation.SetCellData(i, height - 1, lid_data)
    simulation.SetCellData(i, 0, wall_data)

for j in range(height - 1):
    simulation.SetCellData(0, j, wall_data)
    simulation.SetCellData(width - 1, j, wall_data)


print("Running simulation...")
for k in range(simulation_iteration_count):
    simulation.Step(time_step)


print("Drawing results...")
X = np.arange(0.0, 1.0, 0.05)
Y = np.arange(0.0, 1.0, 0.05)
U, V = np.meshgrid(X, Y)

fig, ax = plt.subplots()

for i in range(width):
    for j in range(height):
        vel: Vec2f = simulation.GetVelocity(i, j)
        ax.quiver(1.0 / width * i, 1.0 / height * j, vel.x, vel.y)

plt.show()
