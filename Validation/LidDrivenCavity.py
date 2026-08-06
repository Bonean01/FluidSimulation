import numpy as np
import matplotlib.pyplot as plt

from ctypes import *
from enum import IntEnum


class Vec2f(Structure):
    _fields_ = [
        ("x", c_float),
        ("y", c_float)
    ]

class CellType(IntEnum):
    FLUID = 0
    SOLID = 1

class BoundaryType(IntEnum):
    NONE = 0
    NO_SLIP = 1
    FREE_SLIP = 2
    INLET = 3
    OUTLET = 4

class CellData(Structure):
    _fields_ = [
        ("cell_type", c_uint8),
        ("boundary_type", c_uint8),
        ("prescribed_velocity", Vec2f)
    ]


libc: CDLL = cdll.LoadLibrary("../FluidSolverCore/out/build/debug/FluidSolver.dll")

side_length: int = 33
width: c_int = side_length
height: c_int = side_length
cell_width: c_float = 1.0 / side_length
density: c_float = 1.0
kinematic_viscosity: c_float = 1e-05
iteration_count: c_int = 30

time_step: c_float = 1.0 / 120.0
simulation_iteration_count: int = 1000

wallData: CellData = CellData(CellType.SOLID, BoundaryType.NO_SLIP, Vec2f(0.0, 0.0))
lidData: CellData = CellData(CellType.SOLID, BoundaryType.NO_SLIP, Vec2f(1.0, 0.0))


print("Setting up...")
libc.CreateSimulation.argtypes = [c_int, c_int, c_float, c_float, c_float, c_uint32]
libc.CreateSimulation.restype = c_void_p

handle: c_void_p = libc.CreateSimulation(width, height, cell_width, density, kinematic_viscosity, iteration_count)
libc.SetCellData.argtypes = [c_void_p, c_int, c_int, CellData]
libc.SetCellData.restype = c_void_p
for i in range(width):
    libc.SetCellData(handle, i, height - 1, lidData)
    libc.SetCellData(handle, i, 0, wallData)

for j in range(height - 1):
    libc.SetCellData(handle, 0, j, wallData)
    libc.SetCellData(handle, width - 1, j, wallData)


print("Running simulation...")
libc.Step.argtypes = [c_void_p, c_float]
libc.Step.restype = c_void_p
for k in range(simulation_iteration_count):
    libc.Step(handle, time_step)


print("Drawing results...")
X = np.arange(0.0, 1.0, 0.05)
Y = np.arange(0.0, 1.0, 0.05)
U, V = np.meshgrid(X, Y)

fig, ax = plt.subplots()

libc.GetVelocity.argtypes = [c_void_p, c_int, c_int]
libc.GetVelocity.restype = Vec2f
for i in range(width):
    for j in range(height):
        vel: Vec2f = libc.GetVelocity(handle, i, j)
        ax.quiver(1.0 / width * i, 1.0 / height * j, vel.x, vel.y)

plt.show()
