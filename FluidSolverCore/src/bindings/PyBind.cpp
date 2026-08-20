#include <pybind11/pybind11.h>
#include <pybind11/native_enum.h>

#include <omp.h>

#include "FluidSimulation.h"
#include "utils/profiling/Profiler.h"


namespace py = pybind11;

PYBIND11_MODULE(FluidSolver, m) {
	m.doc() = "FluidSolver python bindings";

	py::native_enum<CellType>(m, "CellType", "enum.Enum")
		.value("FLUID", CellType::Fluid)
		.value("SOLID", CellType::Solid)
		.export_values()
		.finalize();


	py::class_<CellData>(m, "CellData")
		.def(py::init<CellType>(),
			py::arg("cell_type") = CellType::Fluid);


	py::native_enum<BoundaryCondition>(m, "BoundaryCondition", "enum.Enum")
		.value("NONE", BoundaryCondition::None)
		.value("DIRICHLET", BoundaryCondition::Dirichlet)
		.value("HOMOGENEOUS_NEUMANN", BoundaryCondition::HomogeneousNeumann)
		.export_values()
		.finalize();


	py::class_<CellConfig>(m, "CellConfig")
		.def(py::init<CellData, BoundaryData>(),
			py::arg("cell_data") = CellData{},
			py::arg("boundary_data") = BoundaryData{});


	py::class_<FluidSimulation>(m, "FluidSimulation")
		.def(py::init<int, int, float, float, float, unsigned int>(),
			py::arg("width"),
			py::arg("height"),
			py::arg("cell_width"),
			py::arg("density"),
			py::arg("kinematic_viscosity"),
			py::arg("iteration_count"))
		.def("step", &FluidSimulation::step,
			py::arg("time_step"))
		.def("get_density", &FluidSimulation::getDensity)
		.def("get_kinematic_viscosity", &FluidSimulation::getKinematicViscosity)
		.def("get_cell_width", &FluidSimulation::getCellWidth)
		.def("set_cell", py::overload_cast<int, int, CellConfig>(&FluidSimulation::setCell),
			py::arg("i"),
			py::arg("j"),
			py::arg("cell_config"));


	py::class_<Profiler>(m, "Profiler")
		.def("get_task_average_duration", &Profiler::getTaskAverageDuration,
			py::arg("id"))
		.def("get_IDs", &Profiler::getIDs)
		.def("get_instance", &Profiler::getInstance);


	m.def("set_num_threads", &omp_set_num_threads,
		py::arg("n"));
}
