# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project ("FluidSimulation")


# Fetch pybind11 from its git repo
include(FetchContent)
FetchContent_Declare(
	pybind11
	GIT_REPOSITORY https://github.com/pybind/pybind11.git
	GIT_TAG v3.1.0
)
FetchContent_MakeAvailable(pybind11)


# Define a variable that contains the source files
set (SOURCE_FILES
	"include/FluidSimulation.h"

	"include/domain/CellType.h"
	"include/domain/CellData.h"
	"include/domain/BoundaryUtils.h"
	"include/domain/BoundaryCondition.h"
	"include/domain/BoundaryData.h"

	"include/steps/Advection.h"
	"include/steps/Diffusion.h"
	"include/steps/Projection.h"

	"include/math/dataStructures/Vector.h"
	"include/math/dataStructures/Grid.h"
	"include/math/dataStructures/StaggeredGrid.h"
	"include/math/dataStructures/StaggeredGrid.tpp"
	"include/math/dataStructures/ScalarField.h"
	"include/math/dataStructures/VectorField.h"
	"include/math/dataStructures/StaggeredVectorField.h"

	"include/math/operators/FiniteDifference.h"
	"include/math/operators/Staggered.h"

	"include/math/solvers/PressureSolver.h"

	"include/utils/profiling/Profiler.h"
	"include/utils/profiling/ScopeProfiler.h"
	"include/utils/profiling/ProfilingData.h"


	"src/bindings/Exports.cpp"
	
	"src/FluidSimulation.cpp"

	"src/domain/BoundaryUtils.cpp"

	"src/steps/Advection.cpp"
	"src/steps/Diffusion.cpp"
	"src/steps/Projection.cpp"

	"src/math/dataStructures/Vector.cpp"
	"src/math/dataStructures/ScalarField.cpp"
	"src/math/dataStructures/VectorField.cpp"
	"src/math/dataStructures/StaggeredVectorField.cpp"
	
	"src/math/operators/Gradient.cpp"
	"src/math/operators/Divergence.cpp"
	"src/math/operators/Laplacian.cpp"

	"src/math/solvers/PressureSolver.cpp"
)


# Create a library from the project.
add_library (FluidSolver SHARED ${SOURCE_FILES})

# Create a static library for testing.
add_library (FluidSolverStatic STATIC ${SOURCE_FILES})

# Create the python bindings module
pybind11_add_module(FluidSolverPython
    "src/bindings/PyBind.cpp"
)

target_link_libraries(FluidSolverPython
    PRIVATE
        FluidSolverStatic
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET FluidSolver PROPERTY CXX_STANDARD 20)
	set_property(TARGET FluidSolverStatic PROPERTY CXX_STANDARD 20)
	set_property(TARGET FluidSolverPython PROPERTY CXX_STANDARD 20)
endif()


# Setting include directories.
target_include_directories (FluidSolver
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories (FluidSolverStatic
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)


# Link the library with OpenMP
find_package(OpenMP REQUIRED)
target_link_libraries(FluidSolver PRIVATE OpenMP::OpenMP_CXX)

find_package(OpenMP REQUIRED)
target_link_libraries(FluidSolverStatic PRIVATE OpenMP::OpenMP_CXX)


# Compile the testing code
add_subdirectory("testing")
