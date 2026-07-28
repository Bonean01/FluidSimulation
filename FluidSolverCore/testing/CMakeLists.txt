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
set(TEST_SOURCE_FILES
	"unitTests/BiLerpTest.cpp"
	"unitTests/PressureSolverTest.cpp"
)


# Create executable for testing
add_executable (Playground "Playground.cpp")

# Create executable for running the unit tests
add_executable (FluidSimlationTests ${TEST_SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET Playground PROPERTY CXX_STANDARD 20)
	set_property(TARGET FluidSimlationTests PROPERTY CXX_STANDARD 20)
endif()

# Link the test executables with the library
target_link_libraries(Playground PRIVATE FluidSolverStatic)
target_link_libraries(FluidSimlationTests PRIVATE
	FluidSolverStatic
	Catch2::Catch2WithMain
)

# Let Catch2 discover the tests
include(CTest)
include(Catch)
CATCH_DISCOVER_TESTS(FluidSimlationTests)
