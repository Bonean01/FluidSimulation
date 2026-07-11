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


# Create a library from the project.
add_library (FluidSolver SHARED
	
	 "include/FluidSimulation.h" "src/Exports.cpp")

	
if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET FluidSolver PROPERTY CXX_STANDARD 20)
endif()


# Setting include directories.
target_include_directories (FluidSolver
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)
