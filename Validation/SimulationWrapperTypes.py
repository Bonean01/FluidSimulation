import os

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
    VELOCITY_INLET = 3
    VELOCITY_OUTLET = 4

class CellData(Structure):
    _fields_ = [
        ("cell_type", c_uint8),
    ]

class BoundaryData(Structure):
    _fields_ = [
        ("boundary_type", c_uint8),
        ("prescribed_velocity", Vec2f)
    ]

class CellProperties(Structure):
    _fields_ = [
        ("cell_data", CellData),
        ("boundary_data", BoundaryData)
    ]


class FluidSimulation:
    libc: CDLL
    handle: c_void_p

    def __init__(self, width: c_int, height: c_int, cell_width: c_float, density: c_float, kinematic_viscosity: c_float, iteration_count: c_uint32):
        path = "../FluidSolverCore/out/build/debug/"
        if os.path.exists(path + "FluidSolver.dll"):
            self.libc = cdll.LoadLibrary(path + "FluidSolver.dll")
        elif os.path.exists(path + "libFluidSolver.so"):
            self.libc = cdll.LoadLibrary(path + "libFluidSolver.so")
        else:
            raise FileNotFoundError(f"Could not find FluidSolver.dll nor libFluidSolver.so at: {path}")

        self.libc.CreateSimulation.argtypes = [c_int, c_int, c_float, c_float, c_float, c_uint32]
        self.libc.CreateSimulation.restype = c_void_p
        self.handle = self.libc.CreateSimulation(width, height, cell_width, density, kinematic_viscosity, iteration_count)


    def __del__(self):
        self.libc.DestroySimulation.argtypes = [c_void_p]
        self.libc.DestroySimulation.restype = c_void_p
        self.libc.DestroySimulation(self.handle)


    def Step(self, time_step: c_float) -> None:
        self.libc.Step.argtypes = [c_void_p, c_float]
        self.libc.Step.restype = c_void_p
        self.libc.Step(self.handle, time_step)


    def GetVelocity(self, i: c_int, j: c_int) -> Vec2f:
        self.libc.GetVelocity.argtypes = [c_void_p, c_int, c_int]
        self.libc.GetVelocity.restype = Vec2f
        return self.libc.GetVelocity(self.handle, i, j)


    def SetVelocity(self, i: c_int, j: c_int, velocity: Vec2f) -> None:
        self.libc.SetVelocity.argypes = [c_void_p, c_int, c_int, Vec2f]
        self.libc.SetVelocity.restype = c_void_p
        self.libc.SetVelocity(self.handle, i, j, velocity)


    def SetCell(self, i: c_int, j: c_int, cell_properties: CellProperties) -> None:
        self.libc.SetCell.argtypes = [c_void_p, c_int, c_int, CellProperties]
        self.libc.SetCell.restype = c_void_p
        self.libc.SetCell(self.handle, i, j, cell_properties)
