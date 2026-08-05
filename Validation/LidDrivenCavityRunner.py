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
        ("cell_type", CellType),
        ("boundary_type", BoundaryType),
        ("prescribed_velocity", c_float)
    ]


libc = cdll.LoadLibrary("../FluidSolverCore/out/build/debug/libFluidSolver.so")

width = 10
height = 10
cell_width = 1.0
density = 1.0
kinematic_viscosity = 1e-05
iteration_count = 60

libc.CreateSimulation.argtypes = [c_int, c_int, c_float, c_float, c_float, c_uint32]
libc.CreateSimulation.restype = c_void_p
handle = libc.CreateSimulation(width, height, cell_width, density, kinematic_viscosity, iteration_count)

libc.GetVelocity.argtypes = [c_void_p, c_int, c_int]
libc.GetVelocity.restype = Vec2f
vel = libc.GetVelocity(handle, 1, 1)
