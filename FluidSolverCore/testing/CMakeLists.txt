# Create executable for testing
add_executable (FluidSolverTesting "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET FluidSolverTesting PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(FluidSolverTesting PRIVATE FluidSolverStatic)

target_include_directories(FluidSolverTesting
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
