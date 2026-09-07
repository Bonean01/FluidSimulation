# Fluid Simulation
2D Eulerian (grid-based) fluid simulation for incompressible flow that I'm developing for learning the basics of numerical methods and computational fluid dynamics.

The project contains the fluid solver library source code written in C++, an example usage of it to create an interactive visualization using the Unity game engine, and some Python scripts for data analysis, validation and benchmarking of the solver.

No additional libraries have been used so far for the development of the fluid solver besides [Catch2](https://github.com/catchorg/Catch2) for unit testing, with the main reason being that I want to implement the solvers and the data structures myself. 


## Features
The simulation solves the Navier-Stokes equations:

$$\frac{\partial \vec u}{\partial t} = -(\vec{u} · \nabla)\vec{u} - \frac{\nabla p}{\rho} + \nu \nabla^2 \vec{u} + \vec {f}$$
$$\nabla · \vec{u} = 0$$

It discretizes the velocity and pressure fields on a MAC (Marker-And-Cell) grid. It also features semi-Lagrangian advection, and the Jacobi method for solving both the pressure Poisson equation and the viscous diffusion component of the Navier-Stokes equations. My intention is to experiment with other advection schemes that don't cause as much numerical diffusion as well as other faster converging linear solvers. 

For the time being there's two types of cells in the simulation: fluid or solid, and two types of velocity boundary conditions: Dirichlet (for inlets or moving walls) and Homogeneous Neumann (for outflow).

### Visualizer
The visualizer can display both the velocities magnitude and direction as well as the pressure of the fluid, it can also display the values of the smoke field and optionally the divergence of the whole field after projecting for debugging purposes.
| Smoke Field | Pressure Field | Velocity Magnitude |
| :-----------: | :------: | :----------------: |
| ![Smoke](https://github.com/user-attachments/assets/651f3cf1-1a5b-45e0-8e9d-6e9be9a0e25f) | ![Pressure](https://github.com/user-attachments/assets/da067c0d-f9a9-42c6-ac3d-c183fc499f98) | ![Velocity](https://github.com/user-attachments/assets/c06eb986-1cff-432e-8453-2a1b55f7f82c) |


## Validation 
### Lid driven cavity & Poiseuille flow
For the time being the validation scripts only run the simulation for a specified number of iterations and display the results, but I would like to add some way of comparing them with official results to see the error in the solver.

<img width="37%" height="270" alt="LidDrivenCavity" src="https://github.com/user-attachments/assets/9135ebb4-6b10-49bd-9aa7-0a1ee2ee91d0" />
<img width="60%" height="270" alt="DevelopedPoiseuilleFlow" src="https://github.com/user-attachments/assets/7c1374d9-62b7-4595-b8c4-ce81db576f81" />

### Vortex shedding
This result is more of a qualitative observation of the solver where for a constant inflow into a circle, the fluid forms what is known as a [Kármán vortex street](https://en.wikipedia.org/wiki/K%C3%A1rm%C3%A1n_vortex_street) for specific Reynold numbers.
<p align="center">
	<img width="618" height="309" alt="vortex-shedding" src="https://github.com/user-attachments/assets/5618c617-c134-466e-af7f-d8296d5f2d95" />
</p>


## Project structure
```
Analysis/						Contains Python scripts used for validating and benchmarking the solver
FluidSolverCore/
	src/
		bindings/				Provides the C API and Python bindings for external apps
		domain/					Classes used to represent or operate on the fluid's domain
		math/					Mathematical structures and operations used by the simulation
  		steps/					Classes for the individual simulation steps: Advection, Diffusion...
		FluidSimulation.cpp		Defines the order in which simulation steps will be executed
	testing/
		unitTests/				Contains the unit tests
		Playground.cpp			Class containing a main function used for testing purposes

UnityVisualizer/				Root folder of the Unity project containing the visualization
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
Running these commands will generate both a dynamic and a static library as well as an executable for unit testing and another one with the contents of `/FluidSolverCore/testing/Playground.cpp`. To run the Unity visualizer, open the unity editor with version 6000.5.1f1 or higher and import the dynamic library into the Plugins folder located in the `/UnityVisualizer/Assets/` directory. To run the unit tests, run ctest from within `/FluidSimulation/FluidSolverCore/out/build/debug/testing`. For validating or benchmarking the solver run the python modules from the root of the project like such:
```bash
python3 -m Analysis.Validation.LidDrivenCavity
```
## References
- Jos Stam. [Stable Fluids](https://www.josstam.com/_files/ugd/cf1fd6_898fe9b63df946689101b8d074f8efba.pdf)

- Mark J. Harris. [GPU gems chapter 38 - Fast Fluid Dynamics Simulation on the GPU](https://developer.nvidia.com/gpugems/gpugems/part-vi-beyond-triangles/chapter-38-fast-fluid-dynamics-simulation-gpu)

- Robert Bridson, Mathias Müller-Fischer. [Fluid Simulation SIGGRAPH 2007 Course Notes](https://www.cs.ubc.ca/~rbridson/fluidsimulation/fluids_notes.pdf)
