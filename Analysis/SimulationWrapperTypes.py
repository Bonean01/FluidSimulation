import os

from ctypes import *
from enum import IntEnum
from pathlib import Path

class Vec2f(Structure):
    _fields_ = [
        ("x", c_float),
        ("y", c_float)
    ]

class CellType(IntEnum):
    FLUID = 0
    SOLID = 1

class BoundaryCondition(IntEnum):
    NONE = 0
    DIRICHLET = 1
    HOMOGENEOUS_NEUMANN = 2

class CellData(Structure):
    _fields_ = [
        ("cell_type", c_uint8),
    ]

class BoundaryData(Structure):
    _fields_ = [
        ("boundary_type", c_uint8),
        ("prescribed_velocity", Vec2f)
    ]

class CellConfig(Structure):
    _fields_ = [
        ("cell_data", CellData),
        ("boundary_data", BoundaryData)
    ]


class FluidSimulation:
    _libc: CDLL
    _handle: c_void_p
    _grid_width: int
    _grid_height: int
    _cell_width: float

    def __init__(self, width: c_int, height: c_int, cell_width: c_float, density: c_float, kinematic_viscosity: c_float, iteration_count: c_uint32):
        path = "./FluidSolverCore/out/build/debug/"
        if os.path.exists(path + "FluidSolver.dll"):
            self._libc = cdll.LoadLibrary(path + "FluidSolver.dll")
        elif os.path.exists(path + "libFluidSolver.so"):
            self._libc = cdll.LoadLibrary(path + "libFluidSolver.so")
        else:
            raise FileNotFoundError(f"Could not find FluidSolver.dll nor libFluidSolver.so at: {(Path.cwd() / path).resolve()}")

        self._libc.CreateSimulation.argtypes = [c_int, c_int, c_float, c_float, c_float, c_uint32]
        self._libc.CreateSimulation.restype = c_void_p
        self._handle = self._libc.CreateSimulation(width, height, cell_width, density, kinematic_viscosity, iteration_count)

        self._grid_width = width
        self._grid_height = height
        self._cell_width = cell_width


    def __del__(self):
        self._libc.DestroySimulation.argtypes = [c_void_p]
        self._libc.DestroySimulation.restype = c_void_p
        self._libc.DestroySimulation(self._handle)


    def step(self, time_step: c_float) -> None:
        self._libc.Step.argtypes = [c_void_p, c_float]
        self._libc.Step.restype = c_void_p
        self._libc.Step(self._handle, time_step)


    def get_velocity(self, i: c_int, j: c_int) -> Vec2f:
        self._libc.GetVelocity.argtypes = [c_void_p, c_int, c_int]
        self._libc.GetVelocity.restype = Vec2f
        return self._libc.GetVelocity(self._handle, i, j)


    def set_velocity(self, i: c_int, j: c_int, velocity: Vec2f) -> None:
        self._libc.SetVelocity.argypes = [c_void_p, c_int, c_int, Vec2f]
        self._libc.SetVelocity.restype = c_void_p
        self._libc.SetVelocity(self._handle, i, j, velocity)


    def set_cell(self, i: c_int, j: c_int, cell_config: CellConfig) -> None:
        self._libc.SetCell.argtypes = [c_void_p, c_int, c_int, CellConfig]
        self._libc.SetCell.restype = c_void_p
        self._libc.SetCell(self._handle, i, j, cell_config)


    def set_num_threads(self, n: c_int) -> None:
        self._libc.SetNumThreads.argtypes = [c_int]
        self._libc.SetNumThreads.restype = c_void_p
        self._libc.SetNumThreads(n)


    def get_grid_width(self) -> int:
        return self._grid_width


    def get_grid_height(self) -> int:
        return self._grid_height
    

    def get_cell_width(self) -> float:
        return self._cell_width
