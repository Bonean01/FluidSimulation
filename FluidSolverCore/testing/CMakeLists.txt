enable_testing()

# Fetch Catch2 from its git repo
include(FetchContent)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2
	GIT_TAG v3.15.2
)
FetchContent_MakeAvailable(Catch2)


# Define a variable that contains all the test source files
set (TEST_SOURCE_FILES
	"unitTests/BiLerpTest.cpp"
	"unitTests/AdvectionTest.cpp"
	"unitTests/ProjectionTest.cpp"
)

set (TEST_UTILS
	"TestUtils.h"
	"TestUtils.cpp"
)


# Create executable for testing
add_executable (Playground "Playground.cpp"  ${TEST_UTILS})

# Create executable for running the unit tests
add_executable (FluidSimulationTests ${TEST_SOURCE_FILES} ${TEST_UTILS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET Playground PROPERTY CXX_STANDARD 20)
	set_property(TARGET FluidSimulationTests PROPERTY CXX_STANDARD 20)
endif()

# Link the test executables with the library
target_link_libraries(Playground PRIVATE FluidSolverStatic)
target_link_libraries(FluidSimulationTests PRIVATE
	FluidSolverStatic
	Catch2::Catch2WithMain
)

target_include_directories(Playground PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(FluidSimulationTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Let Catch2 discover the tests
include(CTest)
include(Catch)
CATCH_DISCOVER_TESTS(FluidSimulationTests)
