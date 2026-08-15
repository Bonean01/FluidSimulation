# Fluid Simulation
2D Eulerian (grid-based) fluid simulation for incompressible flow that I'm developing for learning the basics of numerical methods and computational fluid dynamics.

The project contains the fluid solver library source code written in C++, an example usage of it to create an interactive visualization using the Unity game engine, and some Python scripts for data analysis and validation of the solver.

No additional libraries have been used so far for the development of the fluid solver besides [Catch2](https://github.com/catchorg/Catch2) for unit testing, with the main reason being that I want to implement the solvers and the data structures myself.

## Features
The simulation discretizes the velocity and pressure fields on a MAC (Marker-And-Cell) grid. It also features semi-Lagrangian advection, and the Jacobi method for solving both the pressure Poisson equation and the viscous diffusion component of the Navier-Stokes equations. My intention is to experiment with other advection schemes that don't cause as much numerical diffusion as well as other faster converging linear solvers. For the time being there's two types of cells in the simulation: fluid or solid, and three types of boundary conditions: NoSlip, VelocityInlet, and VelocityOutlet.
| Smoke Field | Pressure Field | Velocity Magnitude |
| :-----------: | :------: | :----------------: |
| ![Smoke](https://github.com/user-attachments/assets/651f3cf1-1a5b-45e0-8e9d-6e9be9a0e25f) | ![Pressure](https://github.com/user-attachments/assets/da067c0d-f9a9-42c6-ac3d-c183fc499f98) | ![Velocity](https://github.com/user-attachments/assets/c06eb986-1cff-432e-8453-2a1b55f7f82c) |
 
## Project structure
```
FluidSolverCore/src/
	math/					Mathematical structures and operations used by the simulation
  	steps/					Classes for the individual simulation steps: Advection, Diffusion...
	testing/
		unitTests/			Contains the unit tests
		Playground.cpp		Class containing a main function used for testing purposes
	Exports.cpp				Defines the exported API for the library
	FluidSimulation.cpp		Defines the order in which simulation steps will be executed

UnityVisualizer/			Root folder of the Unity project containing the visualization
Validation/					Contains Python scripts used for validating the solver
```
## Building instructions
**Prerequisites**
- CMake version 3.10 or higher.
- [Ninja](https://ninja-build.org/) installed
- C++20 compiler

```bash
git clone https://github.com/Bonean01/FluidSimulation.git
cd FluidSimulation/FluidSolverCore
cmake --preset debug
cmake --build --preset debug
```
Running these commands will generate both a dynamic and a static library as well as an executable for unit testing and another one with the contents of `/FluidSolverCore/testing/Playground.cpp`.
To run the Unity visualizer, open the unity editor with version 6000.5.1f1 or higher and import the dynamic library into the Plugins folder located in the `/UnityVisualizer/Assets/` directory.
To run the unit tests, run `ctest` from within `/FluidSimulation/FluidSolverCore/out/build/debug/testing`.

## References
- Jos Stam. [Stable Fluids](https://www.josstam.com/_files/ugd/cf1fd6_898fe9b63df946689101b8d074f8efba.pdf)

- Mark J. Harris. [GPU gems chapter 38 - Fast Fluid Dynamics Simulation on the GPU](https://developer.nvidia.com/gpugems/gpugems/part-vi-beyond-triangles/chapter-38-fast-fluid-dynamics-simulation-gpu)

- Robert Bridson, Mathias Müller-Fischer. [Fluid Simulation SIGGRAPH 2007 Course Notes](https://www.cs.ubc.ca/~rbridson/fluidsimulation/fluids_notes.pdf)
