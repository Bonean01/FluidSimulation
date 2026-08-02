set (TEST_UTILS
	"../testing/TestUtils.h"
	"../testing/TestUtils.cpp"
)

# Create executable for each validation setup
add_executable(LidDrivenCavity "LidDrivenCavity.cpp" ${TEST_UTILS})


if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET LidDrivenCavity PROPERTY CXX_STANDARD 20)
endif()

# Link the executables with the library
target_link_libraries(LidDrivenCavity PRIVATE FluidSolverStatic)

target_include_directories(LidDrivenCavity PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/testing)
