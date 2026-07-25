# Create executable for testing
add_executable (Playground "Playground.cpp")

# Create executable for each test // temp until catch2
add_executable (BilinearInterpolationTest "unitTests/BiLerpTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET Playground PROPERTY CXX_STANDARD 20)
	set_property(TARGET BilinearInterpolationTest PROPERTY CXX_STANDARD 20)
endif()

# Link the test executables with the library
target_link_libraries(Playground PRIVATE FluidSolverStatic)
target_link_libraries(BilinearInterpolationTest PRIVATE FluidSolverStatic)

# Create the tests
add_test("BilinearInterpolation"  BilinearInterpolationTest)
